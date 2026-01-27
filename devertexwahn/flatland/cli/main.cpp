/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/canvas/svg_canvas.hpp"
#include "flatland/rendering/intersector/quadtree_intersector.hpp"
#include "flatland/rendering/rendering.hpp"
#include "flatland/rendering/scene/load_scene.hpp"

#include "core/logging.hpp"

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage_config.h"

#include "boost/version.hpp"

#include "fmt/core.h"

#include <filesystem>

using namespace de_vertexwahn;

enum ExitStatus {
    Success = 0,
    SceneFileNotFound = 1,
    SensorMissing = 2,
    UnknownError = 9999,
};

void log_boost_version() {
    LOG_INFO_WITH_LOCATION("Using Boost {}.{}.{}",
                           BOOST_VERSION / 100000,      // major version
                           BOOST_VERSION / 100 % 1000,  // minor version
                           BOOST_VERSION % 100  // patch level
    );
}

void plot_quadtree(QuadtreeNode *node, SvgCanvas2f *canvas) {
    if (node == nullptr) {
        return;
    }

    canvas->add(node->bounds);

    for (auto n: node->nodes) {
        plot_quadtree(n, canvas);
    }
}

std::string version_string() { return "Flatland 1.3.6\n"; }

ABSL_FLAG(std::string, scene_filename, "scene.xml", "Okapi scene filename use as input for rendering");

int main(int argc, char **argv) {
    try {
        absl::FlagsUsageConfig flags_config;
        flags_config.version_string = &version_string;
        absl::SetFlagsUsageConfig(flags_config);
        absl::ParseCommandLine(argc, argv);
        std::filesystem::path filename = absl::GetFlag(FLAGS_scene_filename);

        log_boost_version();

        if (!std::filesystem::exists(filename)) {
            LOG_WARNING_WITH_LOCATION("File {} does not exist", filename.string());
            return ExitStatus::SceneFileNotFound;
        }

        LOG_INFO_WITH_LOCATION("Loading scene {}", filename.string());

        auto scene = load_scene2f(filename.string());

        auto sensor = scene->sensor();

        if (sensor == nullptr) {
            LOG_WARNING_WITH_LOCATION("Sensor is missing in scene description");
            return ExitStatus::SensorMissing;
        }

        auto canvas = make_reference_counted<SvgCanvas2f>(
                sensor->film()->width(),
                sensor->film()->height());

        LOG_INFO_WITH_LOCATION("Begin rendering.");

        auto integrator = scene->integrator();
        integrator->set_canvas(canvas);
        render(integrator.get(), canvas, scene.get());

        for (const auto &label: scene->annotations()) {
            canvas->add(label);
        }

        auto intersector = scene->intersector();
        if (intersector->to_string() == "QuadtreeIntersector") {
            ReferenceCounted<QuadtreeIntersector2f> qti = std::dynamic_pointer_cast<QuadtreeIntersector2f>(intersector);
            plot_quadtree(qti->root_node(), canvas.get());
        }

        // determine out path
        std::filesystem::path out_path = fmt::format("{}/{}", filename.parent_path().string(), sensor->film()->filename());

        LOG_INFO_WITH_LOCATION("Store SVG to {}", out_path.string());

        canvas->store(out_path.string());

        LOG_INFO_WITH_LOCATION("Done");
        LOG_INFO_WITH_LOCATION("Shutting down now");
    }
    catch (Exception &ex) {
        LOG_WARNING_WITH_LOCATION("{}", ex.what());
        return ExitStatus::UnknownError;
    }
    catch (std::exception &ex) {
        LOG_WARNING_WITH_LOCATION("{}", ex.what());
        return ExitStatus::UnknownError;
    }

    return ExitStatus::Success;
}
