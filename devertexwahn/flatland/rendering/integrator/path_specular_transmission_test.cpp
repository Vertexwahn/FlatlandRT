/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/integrator/path_specular_transmission.h"
#include "flatland/rendering/rendering.h"
#include "flatland/rendering/scene/load_scene.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(PathSpecularTransmission, Path_render) {
    auto scene = load_scene2f("flatland/scenes/bunny/bunny.flatland.xml");
    ASSERT_TRUE(scene);

    auto sc = make_reference_counted<SvgCanvas2f>(scene->sensor()->film()->size());

    auto integrator = scene->integrator();
    integrator->set_canvas(sc);

    render(integrator.get(), sc, scene.get());

    EXPECT_TRUE(integrator);

    EXPECT_THAT(sc->to_string(), testing::HasSubstr("stroke-width:3"));
}

TEST(PathSpecularTransmission, GivenSceneWhereCameraRayIntersectsDisk_WhenMaxDepthIs2_ThenTraceOnly2Rays) {
    // Arrange
    auto scene = load_scene2f("flatland/scenes/disk.flatland.xml");
    ASSERT_TRUE(scene);

    // Act
    PropertySet ps;
    ps.add_property("max_depth", 2);
    PathSpecularTransmission integrator{ps};

    auto sc = make_reference_counted<SvgCanvas2f>(scene->sensor()->film()->size());

    integrator.set_canvas(sc);
    //integrator.render(scene.get());
    render(&integrator, sc, scene.get());

    // Assert
    EXPECT_THAT(sc->ray_count(), 2u);
}

TEST(PathSpecularTransmission, GivenRectanglesScene_WhenMaxDepthIs7_Then7RaysAreTraced) {
    // Arrange
    auto scene = load_scene2f("flatland/scenes/rectangles.flatland.xml");
    ASSERT_TRUE(scene);

    // Act
    auto integrator = scene->integrator();

    auto canvas = make_reference_counted<SvgCanvas2f>(scene->sensor()->film()->size());

    integrator->set_canvas(canvas);

    render(integrator.get(), canvas, scene.get());

    // Assert
    EXPECT_THAT(canvas->ray_count(), 7u);
}

TEST(PathSpecularTransmission, GivenADisk_WhenRendering_ThenExpect3TraceRays) {
    // Arrange
    auto scene = load_scene2f("flatland/scenes/disk.flatland.xml");
    ASSERT_TRUE(scene);

    auto canvas = make_reference_counted<SvgCanvas2f>(scene->sensor()->film()->size());

    auto integrator = scene->integrator();
    ASSERT_TRUE(integrator);
    integrator->set_canvas(canvas);

    // Act
    render(integrator.get(), canvas, scene.get());

    // Assert
    EXPECT_THAT(canvas->ray_count(), 3u);
}