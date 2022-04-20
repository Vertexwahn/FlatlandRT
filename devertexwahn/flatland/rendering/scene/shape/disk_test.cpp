/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/math/refract.h"
#include "flatland/math/util.h"
#include "flatland/rendering/scene/shape/disk.h"

#include "gmock/gmock.h"

using namespace flatland;

TEST(Disk2f, GivenDiskCenterAndRadius_WhenInitializing_ThenInitializedDisk) {
    // Arrange
    auto diskRadius = 25.f;
    auto diskCenter = Vector2f{100.f, 100.f};
    auto transform = translate(diskCenter);

    PropertySet ps;
    ps.add_property("radius", diskRadius);
    ps.add_property("transform", transform);

    // Act
    Disk2f disk{ps};

    // Assert
    EXPECT_THAT(disk.transform().matrix()(0, 3), diskCenter.x());
    EXPECT_THAT(disk.transform().matrix()(1, 3), diskCenter.y());
    EXPECT_THAT(disk.radius(), diskRadius);
}

// Trivial hit
TEST(Disk2f, GivenADiskAndRay_WhenRayIntersectsDisk_ThenIntersectionOnDisk) {
    // Arrange
    auto diskCenter = Vector2f(500.f, 100.f);
    auto diskRadius = 25.f;
    auto transform = translate(diskCenter);

    PropertySet ps;
    ps.add_property("radius", diskRadius);
    ps.add_property("transform", transform);

    Disk2f disk{ps};

    Point2f ray_origin{0.f, 100.f};
    Vector2f ray_direction{1.f, 0.f};
    Ray2f ray{ray_origin, ray_direction, 0.f, 10000.f};

    // Act
    MediumEvent2f its;
    auto hit = disk.intersect(ray, its);

    // Assert
    EXPECT_TRUE(hit);

    EXPECT_THAT(its.geo_frame.normal.x(), testing::FloatEq(-1.f));
    EXPECT_THAT(its.geo_frame.normal.y(), testing::FloatEq(0.f));
    EXPECT_THAT(its.geo_frame.tangent.x(), testing::FloatEq(0.f));
    EXPECT_THAT(its.geo_frame.tangent.y(), testing::FloatEq(1.f));
}

// Trivial miss
TEST(Disk2f, GivenADiskAndRay_WhenRayMissesDisk_ThenNoIntersectionOnDisk) {
    // Arrange
    auto diskCenter = Vector2f(500.f, 100.f);
    auto diskRadius = 25.f;
    auto transform = translate(diskCenter);

    PropertySet ps;
    ps.add_property("radius", diskRadius);
    ps.add_property("transform", transform);

    Disk2f disk{ps};

    Point2f ray_origin{0.f, 0.f};
    Vector2f ray_direction{1.f, 0.f};
    Ray2f ray{ray_origin, ray_direction, 0.f, 10000.f};

    // Act
    MediumEvent2f its;
    auto hit = disk.intersect(ray, its);

    // Assert
    EXPECT_FALSE(hit);
}

// todo: refactor this test to get nearst intersection or throw it away
TEST(Disk2f, GivenTwoDisksAndRay_WhenRayIntesectsBothDisks_ThenIntersectionOnBothDisksIsValid2) {
    // Arrange
    auto diskCenter1 = Vector2f(100.f, 100.f);
    auto diskCenter2 = Vector2f(300.f, 100.f);
    auto diskRadius = 25.f;

    auto transform1 = translate(diskCenter1);
    auto transform2 = translate(diskCenter2);

    PropertySet ps1;
    ps1.add_property("radius", diskRadius);
    ps1.add_property("transform", transform1);

    PropertySet ps2;
    ps2.add_property("radius", diskRadius);
    ps2.add_property("transform", transform2);

    Disk2f disk1{ps1};
    Disk2f disk2{ps2};

    Point2f ray_origin{0.f, 100.f};
    Vector2f ray_direction{1.f, 0.f};
    Ray2f ray{ray_origin, ray_direction, 0.f, 10000.f};

    // Act
    MediumEvent2f its;
    auto hit1 = disk1.intersect(ray, its);
    auto hit2 = disk2.intersect(ray, its);

    // Assert
    EXPECT_TRUE(hit1);
    EXPECT_TRUE(hit2);
}

// Trivial inner intersection
TEST(Disk2f, GivenADiskAndRay_WhenRayStartsInsideDisk_ThenInnerIntersection) {
    // Arrange
    Vector2f diskCenter{100.f, 100.f};
    Ray2f ray{diskCenter, Vector2f{1.f, 0.f}, 0.f, 100.f};
    auto transform = translate<float>(diskCenter);

    PropertySet ps;
    ps.add_property("radius", 50.f);
    ps.add_property("transform", transform);

    Disk2f disk{ps};

    // Act
    MediumEvent2f its;
    auto result = disk.intersect(ray, its);

    // Assert
    EXPECT_TRUE(result);
}

// todo: no additional value - test different things - remove it?
TEST(Disk2f, GivenADiskAndRay_WhenRayRefracts_ThenInnerIntersection) {
    // Arrange
    Ray2f ray{Point2f{0.f, 100.f}, Vector2f{1.f, 0.f}, 0.f, 100.f};
    auto transform = translate<float>(Vector2f{100.f, 100.f});

    PropertySet ps;
    ps.add_property("radius", 50.f);
    ps.add_property("transform", transform);

    Disk2f disk{ps};

    // Act
    MediumEvent2f its;
    disk.intersect(ray, its);

    // adjusted ray
    ray.max_t = its.t;

    // compute refraction
    Vector2f refracted_direction;
    auto result = refract(ray.direction, its.geo_frame.normal, 1.f / 1.60f, refracted_direction);

    // Assert
    EXPECT_TRUE(result);

    Ray2f refractedRay{its.p + refracted_direction * 0.01f, refracted_direction, 0.f, 200.f};
    result = disk.intersect(refractedRay, its);

    EXPECT_TRUE(result);
    EXPECT_THAT(its.p.x(), 150.f);
    EXPECT_THAT(its.p.y(), 100.f);
}

TEST(Disk2f, GivenADiskAndARay_WhenRayIntersectsSphere_ThenValidNormal) {
    // Arrange
    Ray2f rays[] = {
            {Point2f{-100.f, 0.f},    Vector2f{1.f, 0.f},                        0.f, 1000.f},
            {Point2f{100.f, 0.f},     Vector2f{-1.f, 0.f},                       0.f, 1000.f},
            {Point2f{0.f, 100.f},     Vector2f{0.f, -1.f},                       0.f, 1000.f},
            {Point2f{0.f, -100.f},    Vector2f{0.f, 1.f},                        0.f, 1000.f},
            {Point2f{-100.f, -100.f}, Vector2f{std::sqrt(.5f), std::sqrt(.5f)},  0.f, 1000.f},
            {Point2f{-100.f, 100.f},  Vector2f{std::sqrt(.5f), -std::sqrt(.5f)}, 0.f, 1000.f},
    };
    constexpr int rayCount = sizeof(rays) / sizeof(Ray2f);
    auto transform = identity<float>();

    PropertySet ps;
    ps.add_property("radius", 1.f);
    ps.add_property("transform", transform);

    Disk2f disk{ps};

    // Act
    MediumEvent2f its[rayCount];
    for (int i = 0; i < rayCount; ++i) {
        disk.intersect(rays[i], its[i]);
    }

    // Assert
    EXPECT_THAT(its[0].geo_frame.normal.x(), -1.f);
    EXPECT_THAT(its[0].geo_frame.normal.y(), 0.f);
    EXPECT_THAT(its[1].geo_frame.normal.x(), 1.f);
    EXPECT_THAT(its[1].geo_frame.normal.y(), 0.f);
    EXPECT_THAT(its[2].geo_frame.normal.x(), 0.f);
    EXPECT_THAT(its[2].geo_frame.normal.y(), 1.f);
    EXPECT_THAT(its[3].geo_frame.normal.x(), 0.f);
    EXPECT_THAT(its[3].geo_frame.normal.y(), -1.f);
    EXPECT_THAT(its[4].geo_frame.normal.x(), testing::FloatNear(-std::sqrt(.5f), 0.001f));
    EXPECT_THAT(its[4].geo_frame.normal.y(), testing::FloatNear(-std::sqrt(.5f), 0.001f));
    EXPECT_THAT(its[5].geo_frame.normal.x(), testing::FloatNear(-std::sqrt(.5f), 0.001f));
    EXPECT_THAT(its[5].geo_frame.normal.y(), testing::FloatNear(std::sqrt(.5f), 0.001f));
}

TEST(Intersection2f, Ray2CircleIntersection) {
    float length = 100.f;

    Ray2f r(Point2f(0.f, 100.f), Vector2f(1.f, 0.f), 0.f, length);

    PropertySet ps;
    ps.add_property("radius", 50.f);
    ps.add_property("transform", translate(Vector2f{100.f, 100.f}));

    Disk2f c{ps};

    MediumEvent2f its;
    bool hit = c.intersect(r, its);

    ASSERT_TRUE(hit);
    EXPECT_THAT(its.t, testing::FloatEq(50.f));
}


TEST(RefractionRayHitsSphere, When_RayHitsSphereStraight_Then_ExpectStraightRefraction) {
    // Arrange
    const Ray2f r(Point2f(0.f, 100.f), Vector2f(1.f, 0.f), 0.f, 100.f);

    PropertySet ps;
    ps.add_property("radius", 50.f);
    ps.add_property("transform", translate(Vector2f{100.f, 100.f}));

    const Disk2f disk{ps};

    MediumEvent2f its;
    disk.intersect(r, its);

    Vector2f refracted_direction;

    Vector2f wo = -r.direction;

    // Act
    bool result = refract(wo, its.geo_frame.normal, 1.f / 1.60f, refracted_direction);

    // Assert
    ASSERT_TRUE(result);

    EXPECT_THAT(refracted_direction.dot(Vector2f(1.f, 0.f)), testing::FloatEq(1.f));
}

TEST(RefractionRayHitsSphere, When_RayHitsSphereStraightFromInside_Then_ExpectStraightRefraction) {
    // Arrange
    Ray2f r(Point2f(100.f, 100.f), Vector2f(1.f, 0.f), 0.f, 100.f);

    PropertySet ps;
    ps.add_property("radius", 50.f);
    ps.add_property("transform", translate(Vector2f{100.f, 100.f}));
    Disk2f disk{ps};

    MediumEvent2f its;
    disk.intersect(r, its);

    Vector2f refracted_direction;
    Vector2f wo = -r.direction;

    // Act
    bool result = refract(wo, faceforward(its.geo_frame.normal, wo), 1.f / 1.6f, refracted_direction);

    // Assert
    ASSERT_TRUE(result);

    EXPECT_THAT(its.p.x(), testing::FloatEq(150.f));
    EXPECT_THAT(its.p.y(), testing::FloatEq(100.f));

    EXPECT_THAT(refracted_direction.dot(Vector2f(1.f, 0.f)), testing::FloatEq(1.f));
}

TEST(RefractionRayHitsAndExitsSphere, When_RayFromTopAt45Degrees_HitsSphere) {
    //SvgCanvas2f sc{800, 600};

    Ray2f r{Point2f{50.f, 200.f}, Vector2f{1.f, -1.f}.normalized(), 0.f, 500.f};

    PropertySet ps;
    ps.add_property("radius", 50.f);
    ps.add_property("transform", translate(Vector2f{100.f, 100.f}));
    Disk2f disk{ps};

    //sc.add(&disk);

    MediumEvent2f its;
    disk.intersect(r, its);

    // 100
    // 150
    EXPECT_THAT(its.p.x(), testing::FloatNear(100.f, 0.001f));
    EXPECT_THAT(its.p.y(), testing::FloatNear(150.f, 0.001f));

    r.max_t = its.t;

    //sc.add(r);

    Vector2f refracted_direction;

    Vector2f wo1 = -r.direction;
    refract(wo1, its.geo_frame.normal, 1.f / 1.6f, refracted_direction);

    Ray2f refractedRay{its.p + refracted_direction * 0.01f, refracted_direction, 0.f, 200};

    disk.intersect(refractedRay, its);

    // 139.644
    // 69.5312
    EXPECT_THAT(its.p.x(), testing::FloatNear(139.644f, 0.001f));
    EXPECT_THAT(its.p.y(), testing::FloatNear(69.5312f, 0.001f));

    refractedRay.max_t = its.t;
    //sc.add(refractedRay);

    Vector2f refracted_direction2;
    Vector2f wo = -refractedRay.direction;
    refract(wo, faceforward(its.geo_frame.normal, wo), 1.6f / 1.f, refracted_direction2);

    Ray2f refractedRay2{its.p, refracted_direction2, 0.f, 50.f};
    //sc.add(refractedRay2);
}
