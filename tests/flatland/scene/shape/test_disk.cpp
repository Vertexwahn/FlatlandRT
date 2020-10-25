/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/scene/shape/disk.h"
#include "flatland/math/util.h"
#include "flatland/math/refraction.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(Disk2f, GivenDiskCenterAndRadius_WhenInitializing_ThenInitializedDisk) {
    // Arrange
    auto diskRadius = 25.0f;
    auto diskCenter = Vector2f{100.0f, 100.0f};
    auto transform = translate(diskCenter);

    // Act
    Disk2f disk{transform, diskRadius};

    // Assert
    EXPECT_THAT(disk.getTransform().getMatrix()(0,3), diskCenter.x());
    EXPECT_THAT(disk.getTransform().getMatrix()(1,3), diskCenter.y());
    EXPECT_THAT(disk.getRadius(), diskRadius);
}

TEST(Disk2f, GivenDiskAndRay_WhenRayIntesectsDisk_ThenIntersectionIsValid) {
    // Arrange
    auto diskCenter = Vector2f(100.0f, 100.0f);
    auto diskRadius = 25.0f;
    auto transform = translate(diskCenter);

    Disk2f disk{transform, diskRadius};

    Point2f rayOrigin{0.0f, 100.0f};
    Vector2f rayDirection{1.0f, 0.0f};
    Ray2f ray{rayOrigin, rayDirection, 0.0f, 10000.0f};

    // Act
    MediumEvent2f its;
    auto hit = disk.intersect(ray, its);

    // Assert
    EXPECT_TRUE(hit);
}

// Trivial hit
TEST(Disk2f, GivenADiskAndRay_WhenRayIntesectsDisk_ThenIntersectionOnDisk) {
    // Arrange
    auto diskCenter = Vector2f(500.0f, 100.0f);
    auto diskRadius = 25.0f;
    auto transform = translate(diskCenter);

    Disk2f disk{transform, diskRadius};

    Point2f rayOrigin{0.0f, 100.0f};
    Vector2f rayDirection{1.0f, 0.0f};
    Ray2f ray{rayOrigin, rayDirection, 0.0f, 10000.0f};

    // Act
    MediumEvent2f its;
    auto hit = disk.intersect(ray, its);

    // Assert
    EXPECT_TRUE(hit);
}

// Trivial miss
TEST(Disk2f, GivenADiskAndRay_WhenRayMissesDisk_ThenNoIntersectionOnDisk) {
    // Arrange
    auto diskCenter = Vector2f(500.0f, 100.0f);
    auto diskRadius = 25.0f;
    auto transform = translate(diskCenter);

    Disk2f disk{transform, diskRadius};

    Point2f rayOrigin{0.0f, 0.0f};
    Vector2f rayDirection{1.0f, 0.0f};
    Ray2f ray{rayOrigin, rayDirection, 0.0f, 10000.0f};

    // Act
    MediumEvent2f its;
    auto hit = disk.intersect(ray, its);

    // Assert
    EXPECT_FALSE(hit);
}

// todo: refactor this test to get nearst intersection or throw it away
TEST(Disk2f, GivenTwoDisksAndRay_WhenRayIntesectsBothDisks_ThenIntersectionOnBothDisksIsValid2) {
    // Arrange
    auto diskCenter1 = Vector2f(100.0f, 100.0f);
    auto diskCenter2 = Vector2f(300.0f, 100.0f);
    auto diskRadius = 25.0f;

    auto transform1 = translate(diskCenter1);
    auto transform2 = translate(diskCenter2);

    Disk2f disk1{transform1, diskRadius};
    Disk2f disk2{transform2, diskRadius};

    Point2f rayOrigin{0.0f, 100.0f};
    Vector2f rayDirection{1.0f, 0.0f};
    Ray2f ray{rayOrigin, rayDirection, 0.0f, 10000.0f};

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
    Point2f diskCenter{100.0f, 100.0f};
    Ray2f ray{diskCenter, Vector2f{1.0f, 0.0f}, 0.0f, 100.0f};
    auto transform = translate<float>(diskCenter);
    Disk2f disk{transform, 50.0f};

    // Act
    MediumEvent2f its;
    auto result = disk.intersect(ray, its);

    // Assert
    EXPECT_TRUE(result);
}

// todo: no additional value - test different things - remove it?
TEST(Disk2f, GivenADiskAndRay_WhenRayRefracts_ThenInnerIntersection) {
    // Arrange
    Ray2f ray{Point2f{0.0f, 100.0f}, Vector2f{1.0f, 0.0f}, 0.0f, 100.0f};
    auto transform = translate<float>(Point2f{100.0f, 100.0f});
    Disk2f disk{transform, 50.0f};

    // Act
    MediumEvent2f its;
    disk.intersect(ray, its);

    // adjusted ray
    ray.max_t = its.t;

    // compute refraction
    Vector2f refractedDirection;
    auto result = refract(ray.direction, its.n, 1.0f / 1.60f, refractedDirection);

    // Assert
    EXPECT_TRUE(result);

    Ray2f refractedRay{its.p + refractedDirection * 0.01f, refractedDirection, 0.0f, 200.0f};
    result = disk.intersect(refractedRay, its);

    EXPECT_TRUE(result);
    EXPECT_THAT(its.p.x(), 150.0f);
    EXPECT_THAT(its.p.y(), 100.0f);
}

TEST(Disk2f, GivenADiskAndARay_WhenRayIntersectsSphere_ThenValidNormal) {
    // Arrange
    Ray2f rays[] = {
        {Point2f{-100.0f, 0.0f}, Vector2f{1.0f, 0.0f}, 0.0f, 1000.0f},
        {Point2f{100.0f, 0.0f}, Vector2f{-1.0f, 0.0f}, 0.0f, 1000.0f},
        {Point2f{0.0f, 100.0f}, Vector2f{0.0f, -1.0f}, 0.0f, 1000.0f},
        {Point2f{0.0f, -100.0f}, Vector2f{0.0f, 1.0f}, 0.0f, 1000.0f},
        {Point2f{-100.0f, -100.0f}, Vector2f{std::sqrt(0.5f), std::sqrt(0.5f)}, 0.0f, 1000.0f},
        {Point2f{-100.0f, 100.0f}, Vector2f{std::sqrt(0.5f), -std::sqrt(0.5f)}, 0.0f, 1000.0f},
    };
    constexpr int rayCount = sizeof(rays)/ sizeof(Ray2f);
    auto transform = identity<float>();
    Disk2f disk{transform, 1.0f};

    // Act
    MediumEvent2f its[rayCount];
    for(int i = 0; i < rayCount; ++i) {
        disk.intersect(rays[i], its[i]);
    }

    // Assert
    EXPECT_THAT(its[0].n.x(), -1.0f);
    EXPECT_THAT(its[0].n.y(), 0.0f);
    EXPECT_THAT(its[1].n.x(), 1.0f);
    EXPECT_THAT(its[1].n.y(), 0.0f);
    EXPECT_THAT(its[2].n.x(), 0.0f);
    EXPECT_THAT(its[2].n.y(), 1.0f);
    EXPECT_THAT(its[3].n.x(), 0.0f);
    EXPECT_THAT(its[3].n.y(), -1.0f);
    EXPECT_THAT(its[4].n.x(), testing::FloatNear(-std::sqrt(0.5f), 0.001f));
    EXPECT_THAT(its[4].n.y(), testing::FloatNear(-std::sqrt(0.5f), 0.001f));
    EXPECT_THAT(its[5].n.x(), testing::FloatNear(-std::sqrt(0.5f), 0.001f));
    EXPECT_THAT(its[5].n.y(), testing::FloatNear(std::sqrt(0.5f), 0.001f));
}