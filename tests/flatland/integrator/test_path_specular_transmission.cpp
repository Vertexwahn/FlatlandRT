/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/integrator/path_specular_transmission.h"

#include "flatland/scene/load_scene.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(PathSpecularTransmission, Path_render) {
    auto scene = loadScene("scenes/bunny.flatland.xml");
    ASSERT_TRUE(scene);

    auto sc = makeReferenceCounted<SvgCanvas2f>(scene->getCamera()->getFilm().getSize());

    auto integrator = scene->getIntegrator();
    integrator->setCanvas(sc);
    integrator->render(scene.get());

    EXPECT_TRUE(integrator);

    EXPECT_THAT(sc->toString(), ::testing::HasSubstr("stroke-width:3"));
}

TEST(PathSpecularTransmission, GivenSceneWhereCameraRayIntersectsDisk_WhenMaxDepthIs2_ThenTraceOnly2Rays) {
    // Arrange
    auto scene = loadScene("scenes/disk.flatland.xml");
    ASSERT_TRUE(scene);

    // Act
    PropertySet ps;
    ps.addProperty("max_depth", 2);
    PathSpecularTransmission integrator{ps};

    auto sc = makeReferenceCounted<SvgCanvas2f>(scene->getCamera()->getFilm().getSize());

    integrator.setCanvas(sc);
    integrator.render(scene.get());

    // Assert
    EXPECT_THAT(sc->getRayCount(), 2u);
}

TEST(PathSpecularTransmission, GivenRectanglesScene_WhenMaxDepthIs7_Then7RaysAreTraced) {
    // Arrange
    auto scene = loadScene("scenes/rectangles.flatland.xml");
    ASSERT_TRUE(scene);

    // Act
    auto integrator = scene->getIntegrator();

    auto sc = makeReferenceCounted<SvgCanvas2f>(scene->getCamera()->getFilm().getSize());

    integrator->setCanvas(sc);
    integrator->render(scene.get());

    // Assert
    EXPECT_THAT(sc->getRayCount(), 7u);
}

TEST(PathSpecularTransmission, GivenADisk_WhenRendering_ThenExpect3TraceRays) {
    // Arrange
    auto scene = loadScene("scenes/disk.flatland.xml");
    ASSERT_TRUE(scene);

    auto sc = makeReferenceCounted<SvgCanvas2f>(scene->getCamera()->getFilm().getSize());

    auto integrator = scene->getIntegrator();
    ASSERT_TRUE(integrator);
    integrator->setCanvas(sc);

    // Act
    integrator->render(scene.get());

    // Assert
    EXPECT_THAT(sc->getRayCount(), 3u);
}