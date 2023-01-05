/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/integrator/ambient_occlusion.h"
#include "flatland/rendering/scene/load_scene.h"
#include "flatland/rendering/rendering.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(AmbientOcclusion2, TestBackgroundColor) {
    // Arrange
    Scene2f scene;
    class NullIntersector : public IntersectorType<2, float> {
    public:
        NullIntersector() = default;

        virtual void build_acceleration_structure(std::vector<ReferenceCounted<Shape>> shapes) override {};

        virtual bool intersect(const Ray &ray, MediumEvent &me) const override { return false; };
    };
    auto intersector = make_reference_counted<NullIntersector>();
    scene.set_intersector(intersector);

    Ray2f ray{{0.f, 0.f}, {1.f, 0.f}, 0.f, 100.f};

    PropertySet ps_sampler;
    IndependentSampler sampler{ps_sampler};

    PropertySet ps;
    ps.add_property("background_color", Color3f{1.f, .5f, .1f});

    // Act
    AmbientOcclusion2f aoi{ps};
    Color3f color = aoi.trace(&scene, &sampler, ray, 1);

    // Assert
    EXPECT_THAT(color.red(), 1.f);
    EXPECT_THAT(color.green(), .5f);
    EXPECT_THAT(color.blue(), 0.1f);
}

TEST(AmbientOcclusion2, AmbientOcclusion_render) {
    // Arrange
    auto scene = load_scene2f("flatland/scenes/ao.flatland.xml");
    ASSERT_TRUE(scene);
    auto canvas = make_reference_counted<SvgCanvas2f>(scene->sensor()->film()->size());
    auto integrator = scene->integrator();
    integrator->set_canvas(canvas);
    ASSERT_TRUE(integrator);
    auto sampler = scene->sampler();
    ASSERT_TRUE(sampler.get());

    // Act
    auto ray = scene->sensor()->generate_ray(Point2f{0.f, 0.f});
    Color3f color = integrator->trace(scene.get(), sampler.get(), ray, 5);

    render(integrator.get(), canvas, scene.get());

    // Assert
    EXPECT_THAT(canvas->shape_count(), 1u);
    EXPECT_THAT(color.red(), testing::FloatEq(1.f));
    EXPECT_THAT(color.green(), testing::FloatEq(1.f));
    EXPECT_THAT(color.blue(), testing::FloatEq(1.f));
}

TEST(MediumEvent2, GivenFrameWithRectangle_WhenRayIntersectsScene_ExpectCorrectFrame) {
    // Arrange
    auto scene = load_scene2f("flatland/scenes/ao.flatland.xml");
    ASSERT_TRUE(scene);

    // Act
    auto ray = scene->sensor()->generate_ray(Point2f{0.f, 0.f});
    MediumEvent2f me;
    scene->intersect(ray, me);

    // Assert
    EXPECT_THAT(me.geo_frame.normal.x(), testing::FloatEq(me.geo_frame.normal.x()));
    EXPECT_THAT(me.geo_frame.normal.y(), testing::FloatEq(me.geo_frame.normal.y()));
}

TEST(AmbientOcclusion2, GivenASceneWithARectangle_WhenHitingRectangle_ThenExpectNoOcclusion) {
    // Arrange
    auto scene = load_scene2f("flatland/scenes/ao.flatland.xml");
    ASSERT_TRUE(scene);

    auto canvas = make_reference_counted<SvgCanvas2f>(scene->sensor()->film()->size());

    auto integrator = scene->integrator();
    ASSERT_TRUE(integrator);
    integrator->set_canvas(canvas);

    // Act
    render(integrator.get(), canvas, scene.get());

    // Assert
    EXPECT_THAT(canvas->ray_count(), testing::Ge(2u));
}
