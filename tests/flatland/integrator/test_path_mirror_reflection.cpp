/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/scene/load_scene.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(PathMirrorReflection, GivenSceneWithMirror_WhenRayHitsMirror_ExpectReflection) {
    // Arrange
    auto scene = loadScene("scenes/mirror.flatland.xml");
    ASSERT_TRUE(scene);
    auto sc = makeReferenceCounted<SvgCanvas2f>(scene->getCamera()->getFilm().getSize());
    auto integrator = scene->getIntegrator();
    integrator->setCanvas(sc);
    ASSERT_TRUE(integrator);

    // Act
    integrator->render(scene.get());

    // Assert
    EXPECT_THAT(sc->getShapeCount(), 1u);
    EXPECT_THAT(sc->getRayCount(), 2u);
    EXPECT_FLOAT_EQ(sc->getRays()[0].direction.x(), sqrt(0.5f));
    EXPECT_FLOAT_EQ(sc->getRays()[0].direction.y(), sqrt(0.5f));
    EXPECT_FLOAT_EQ(sc->getRays()[1].direction.x(), sqrt(0.5f));
    EXPECT_FLOAT_EQ(sc->getRays()[1].direction.y(), -sqrt(0.5f));
}

// todo: reflection inside a shape?