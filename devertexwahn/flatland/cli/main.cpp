/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/core/logging.h"
#include "flatland/rendering/canvas/svg_canvas.h"
#include "flatland/rendering/intersector/quadtree_intersector.h"
#include "flatland/rendering/rendering.h"
#include "flatland/rendering/scene/load_scene.h"

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage_config.h"
#include "boost/version.hpp"
#include "fmt/core.h"
#include "gflags/gflags.h"

#include <filesystem>

using namespace flatland;

enum ExitStatus {
    Success = 0,
    SceneFileNotFound = 1,
    SensorMissing = 2,
    UnknownError = 9999,
};

void log_boost_version() {
    LOG(INFO) << "Using Boost "
              << BOOST_VERSION / 100000 << "."      // major version
              << BOOST_VERSION / 100 % 1000 << "."  // minor version
              << BOOST_VERSION % 100                // patch level
              << std::endl;
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

std::string version_string() { return "Flatland 1.3.5\n"; }

ABSL_FLAG(std::string, scene_filename, "scene.xml", "Okapi scene filename use as input for rendering");

int main(int argc, char **argv) {
    try {
        FLAGS_logtostderr = true;
        //google::SetLogDestination(google::GLOG_INFO,"log.txt");
        google::InitGoogleLogging(argv[0]);
        //gflags::ParseCommandLineFlags(&argc, &argv, true);

        absl::FlagsUsageConfig flags_config;
        flags_config.version_string = &version_string;
        absl::SetFlagsUsageConfig(flags_config);
        absl::ParseCommandLine(argc, argv);
        std::filesystem::path filename = absl::GetFlag(FLAGS_scene_filename);

        log_boost_version();

        if (!std::filesystem::exists(filename)) {
            LOG(ERROR) << "File " << filename << " does not exist";
            return ExitStatus::SceneFileNotFound;
        }

        LOG(INFO) << "Loading scene " << std::filesystem::path(filename) << ".";

        auto scene = load_scene2f(filename.string());

        auto sensor = scene->sensor();

        if (sensor == nullptr) {
            LOG(ERROR) << "Sensor is missing in scene description";
            return ExitStatus::SensorMissing;
        }

        auto canvas = make_reference_counted<SvgCanvas2f>(
                sensor->film()->width(),
                sensor->film()->height());

        LOG(INFO) << "Begin rendering.";

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

        LOG(INFO) << "Store SVG to " << out_path << ".";

        canvas->store(out_path.string());

        LOG(INFO) << "Done.";
        LOG(INFO) << "Shutting down now.";
    }
    catch (FlatlandException &ex) {
        LOG(ERROR) << ex.what();
        return ExitStatus::UnknownError;
    }
    catch (std::exception &ex) {
        LOG(ERROR) << ex.what();
        return ExitStatus::UnknownError;
    }

    return ExitStatus::Success;
}
