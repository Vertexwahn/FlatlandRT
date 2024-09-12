/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/integrator/path_mirror_reflection.hpp"
#include "flatland/rendering/rendering.hpp"
#include "flatland/rendering/scene/load_scene.hpp"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(PathMirrorReflection, GivenSceneWithMirror_WhenRayHitsMirror_ExpectReflection) {
    // Arrange
    auto scene = load_scene2f("flatland/scenes/mirror.flatland.xml");
    ASSERT_TRUE(scene);
    auto sc = make_reference_counted<SvgCanvas2f>(scene->sensor()->film()->size());
    auto integrator = scene->integrator();
    integrator->set_canvas(sc);
    ASSERT_TRUE(integrator);

    // Act
    render(integrator.get(), sc, scene.get());

    // Assert
    EXPECT_THAT(sc->shape_count(), 1u);
    EXPECT_THAT(sc->ray_count(), 2u);
    EXPECT_FLOAT_EQ(sc->rays()[0].direction.x(), sqrt(.5f));
    EXPECT_FLOAT_EQ(sc->rays()[0].direction.y(), sqrt(.5f));
    EXPECT_FLOAT_EQ(sc->rays()[1].direction.x(), sqrt(.5f));
    EXPECT_FLOAT_EQ(sc->rays()[1].direction.y(), -sqrt(.5f));
}


TEST(PathMirrorReflection2, trace) {
    PropertySet ps;
    ps.add_property("max_depth", 5);
    PathMirrorReflection2f path_mirror_reflection{ps};

    Ray2f ray{Point2f{0.f, 0.f}, Vector2f{1.f, 0.f}, 0.f, 20000.f};
    auto color = path_mirror_reflection.trace(
            nullptr,
            nullptr,
            ray,
            100);

    EXPECT_THAT(color.red(), testing::FloatNear(0.0, 0.01));
}

// todo: reflection inside a shape?
