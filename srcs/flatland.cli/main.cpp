/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/canvas/svg_canvas.h"
#include "flatland/core/filesystem.h"
#include "flatland/scene/load_scene.h"

#include <tclap/CmdLine.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <iostream>
#include <variant>

using namespace Flatland;

enum class ReturnStatus : int {
    Success = 0,
    SceneFileNotFound = 1,
    OtherError = 2
};

int main(int argc, char **argv) {
    try {
        FLAGS_logtostderr = true;
        //google::SetLogDestination(google::GLOG_INFO,"log.txt");
        google::InitGoogleLogging(argv[0]);
        gflags::ParseCommandLineFlags(&argc, &argv, true);

        TCLAP::CmdLine cmd("Flatland command line interface", ' ', "1.0");

        TCLAP::UnlabeledValueArg<std::string>  sceneFilename(
                "filename",
                "Flatland scene filename",
                true,
                "scene.xml",
                "Flatland scene filename use as input for rendering");

        cmd.add(sceneFilename);

        cmd.parse(argc, argv);

        std::string_view filename = sceneFilename.getValue();

        if(!fs::exists(filename)) {
            LOG(ERROR) << "File " << filename << " does not exist";
            std::cout << "File " << filename << " does not exist" << std::endl;
            return 0; //ReturnStatus::SceneFileNotFound;
        }

        LOG(INFO) << "Loading scene " << fs::path(filename) << ".";

        auto scene = loadScene(filename);

        ReferenceCounted<Camera2f> camera = scene->getCamera();

        if(scene->getCamera() == nullptr) {
            LOG(ERROR) << "Camera is missing in scene description";
            return -2;
        }

        auto sc = makeReferenceCounted<SvgCanvas2f>(
                scene->getCamera()->getFilm().getWidth(),
                scene->getCamera()->getFilm().getHeight());

        LOG(INFO) << "Begin rendering.";

        ReferenceCounted<Integrator2f> integrator = scene->getIntegrator();
        integrator->setCanvas(sc);
        integrator->render(scene.get());

        for(const auto& label : scene->getAnnotations()) {
            sc->add(label);
        }

        // determine out path
        fs::path p(filename);
        std::stringstream ss;
        ss << p.parent_path().string() << "/" << scene->getCamera()->getFilm().getFilename();
        fs::path outPath = ss.str();

        LOG(INFO) << "Store SVG to " << outPath << ".";

        sc->store(outPath.string());

        LOG(INFO) << "Done.";
        LOG(INFO) << "Shutting down now.";

        return 0;
    }
    catch (TCLAP::ArgException &ex) {
        std::cerr << "error: " << ex.error() << " for arg " << ex.argId() << std::endl;
    }
    catch (LoadSceneException &ex) {
        LOG(ERROR) << ex.what();
    }
    catch (std::exception & ex) {
        LOG(ERROR) << ex.what();
    }

    return 0;
}