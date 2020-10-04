/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/scene/load_scene.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(AmbientOcclusion, AmbientOcclusion_render) {
    // Arrange
    auto scene = loadScene("scenes/ao.flatland.xml");
    ASSERT_TRUE(scene);
    auto sc = makeReferenceCounted<SvgCanvas2f>(scene->getCamera()->getFilm().getSize());
    auto integrator = scene->getIntegrator();
    integrator->setCanvas(sc);
    ASSERT_TRUE(integrator);

    // Act
    auto ray = scene->getCamera()->generateRay();
    Color3f color = integrator->trace(scene.get(), ray, 5);

    integrator->render(scene.get());

    // Assert
    EXPECT_THAT(sc->getShapeCount(), 1u);
    EXPECT_THAT(color.red(), ::testing::FloatEq(1.0f));
}