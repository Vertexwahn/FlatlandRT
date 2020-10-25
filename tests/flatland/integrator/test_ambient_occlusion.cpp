/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/scene/load_scene.h"
//#include "flatland/scene/scene.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(AmbientOcclusion, AmbientOcclusion_render) {
    // Arrange
    auto scene = loadScene("scenes/ao.flatland.xml");
    ASSERT_TRUE(scene);
    auto canvas = makeReferenceCounted<SvgCanvas2f>(scene->getCamera()->getFilm().getSize());
    auto integrator = scene->getIntegrator();
    integrator->setCanvas(canvas);
    ASSERT_TRUE(integrator);

    // Act
    auto ray = scene->getCamera()->generateRay();
    Color3f color = integrator->trace(scene.get(), ray, 5);

    integrator->render(scene.get());

    // Assert
    EXPECT_THAT(canvas->getShapeCount(), 1u);
    EXPECT_THAT(color.red(), testing::FloatEq(1.0f));
    EXPECT_THAT(color.green(), testing::FloatEq(1.0f));
    EXPECT_THAT(color.blue(), testing::FloatEq(1.0f));
}

TEST(MediumEvent, toWorld1) {
    // Arrange
    MediumEvent2f me;
    me.frame.tangent = Vector2f{1.0f, 0.0f};
    me.frame.normal = Vector2f{0.0f, 1.0f};

    Vector2f v{1.0f, 1.0f};

    // Act
    Vector2f vt = me.frame.toWorld(v);

    // Assert
    EXPECT_THAT(vt.x(), testing::FloatEq(1.0f));
    EXPECT_THAT(vt.y(), testing::FloatEq(1.0f));
}

TEST(MediumEvent, toWorld2) {
    // Arrange
    MediumEvent2f me;
    me.frame.tangent = Vector2f{0.0f, 1.0f};
    me.frame.normal = Vector2f{-1.0f, 0.0f};

    Vector2f v{1.0f, 1.0f};

    // Act
    Vector2f vt = me.frame.toWorld(v);

    // Assert
    EXPECT_THAT(vt.x(), testing::FloatEq(-1.0f));
    EXPECT_THAT(vt.y(), testing::FloatEq(1.0f));
}

TEST(Rectangle, GivenFrameWithRectangle_WhenRayIntersectsScene_ExpectCorrectFrame) {
    // Arrange
    auto scene = loadScene("scenes/ao.flatland.xml");
    ASSERT_TRUE(scene);

    // Act
    auto ray = scene->getCamera()->generateRay();
    MediumEvent2f me;
    scene->intersect(ray, me);

    // Assert
    EXPECT_THAT(me.n.x(), testing::FloatEq(me.frame.normal.x()));
    EXPECT_THAT(me.n.y(), testing::FloatEq(me.frame.normal.y()));
}

TEST(AmbientOcclusion, GivenASceneWithARectangle_WhenHitingRectangle_ThenExpectNoOcclusion) {
    // Arrange
    auto scene = loadScene("scenes/ao.flatland.xml");
    ASSERT_TRUE(scene);

    auto canvas = makeReferenceCounted<SvgCanvas2f>(scene->getCamera()->getFilm().getSize());

    auto integrator = scene->getIntegrator();
    ASSERT_TRUE(integrator);
    integrator->setCanvas(canvas);

    // Act
    integrator->render(scene.get());

    // Assert
    EXPECT_THAT(canvas->getRayCount(), testing::Ge(2u));
}
