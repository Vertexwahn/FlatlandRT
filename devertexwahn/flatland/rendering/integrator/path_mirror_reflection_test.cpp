/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/scene/load_scene.h"
#include "flatland/rendering/rendering.h"

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

// todo: reflection inside a shape?