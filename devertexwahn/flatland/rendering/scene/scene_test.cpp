/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/scene/load_scene.hpp"

#include "flatland/rendering/shape/disk.hpp"
#include "core/reference_counted.hpp"
#include "flatland/rendering/intersector/brute_force_intersector.hpp"
#include "flatland/rendering/property_set.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(Scene, GivenAScene_WhenAddingAAnnotations_ThenValidAnnotion) {
    // Arrange
    Scene2f scene;
    float x = 100.f;
    float y = 200.f;
    std::string text = "Text";

    // Act
    scene.add_annotation(Label2f{{x, y}, text});

    // Assert
    EXPECT_THAT(scene.annotation_count(), 1u);
    EXPECT_THAT(scene.annotations()[0].position.x(), testing::FloatEq(x));
    EXPECT_THAT(scene.annotations()[0].position.y(), testing::FloatEq(y));
    EXPECT_THAT(scene.annotations()[0].text, text);
}

TEST(Scene, GivenRayThatHitsASceneWithDisk_ThenIntersectedDisk) {
    // Arrange
    auto transform = translate(Vector2f{100.f, 100.f});

    PropertySet ps;
    ps.add_property("radius", 25.f);
    ps.add_property("transform", transform);
    auto disk = make_reference_counted<Disk2f>(ps);
    auto scene = make_reference_counted<Scene2f>();
    scene->add_shape(disk);

    auto intersector = make_reference_counted<BruteForceIntersector2f>(PropertySet{});
    scene->set_intersector(intersector);
	intersector->build_acceleration_structure(scene->shapes());

    Point2f ray_origin{0.f, 100.f};
    Vector2f ray_direction{1.f, 0.f};
    ray_direction.normalize();
    Ray2f ray(ray_origin, ray_direction, 0.f, 10000.f);

    // Act
    MediumEvent2f its;
    bool hit = scene->intersect(ray, its);

    // Assert
    EXPECT_TRUE(hit);
    EXPECT_THAT(its.p, (Point2f{75.f, 100.f}));
}

