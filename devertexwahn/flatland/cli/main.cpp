/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/core/logging.h"
#include "flatland/rendering/canvas/svg_canvas.h"
#include "flatland/rendering/intersector/quadtree_intersector.h"
#include "flatland/rendering/rendering.h"
#include "flatland/rendering/scene/load_scene.h"

#include "boost/version.hpp"
#include "fmt/core.h"
#include "gflags/gflags.h"
#include "tclap/CmdLine.h"

#include <filesystem>

using namespace flatland;

enum ExitStatus {
    Success = 0,
    SceneFileNotFound = 1,
    SensorMissing = 2,
};

void print_boost_version() {
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

int main(int argc, char **argv) {
    try {
        FLAGS_logtostderr = true;
        //google::SetLogDestination(google::GLOG_INFO,"log.txt");
        google::InitGoogleLogging(argv[0]);
        gflags::ParseCommandLineFlags(&argc, &argv, true);

        TCLAP::CmdLine cmd("Flatland command line interface", ' ', "1.3.0");

        TCLAP::UnlabeledValueArg <std::string> scene_filename(
                "filename",
                "Flatland scene filename",
                true,
                "scene.xml",
                "Flatland scene filename use as input for rendering");

        cmd.add(scene_filename);

        cmd.parse(argc, argv);

        print_boost_version();

        std::string_view filename = scene_filename.getValue();

        if (!std::filesystem::exists(filename)) {
            LOG(ERROR) << "File " << filename << " does not exist";
            return ExitStatus::SceneFileNotFound;
        }

        LOG(INFO) << "Loading scene " << std::filesystem::path(filename) << ".";

        auto scene = load_scene2f(filename);

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
        std::filesystem::path p(filename);
        std::filesystem::path out_path = fmt::format("{}/{}", p.parent_path().string(), sensor->film()->filename());

        LOG(INFO) << "Store SVG to " << out_path << ".";

        canvas->store(out_path.string());

        LOG(INFO) << "Done.";
        LOG(INFO) << "Shutting down now.";
    }
    catch (TCLAP::ArgException &ex) {
        std::cerr << "error: " << ex.error() << " for arg " << ex.argId() << std::endl;
    }
    catch (LoadSceneException &ex) {
        LOG(ERROR) << ex.what();
    }
    catch (std::exception &ex) {
        LOG(ERROR) << ex.what();
    }

    return ExitStatus::Success;
}
