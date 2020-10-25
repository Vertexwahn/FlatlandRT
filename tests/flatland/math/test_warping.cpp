/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/math/warping.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(Warping, warpUniformSquareToConcentricDisk) {
    Point2f sample[] = {
            {0.0f, 0.0f},
            {0.5f, 0.5f},
            {1.0f, 1.0f},
    };

    Point2f expectedWarpSamples[] = {
            {-0.707107f, -0.707107f},
            {0.0f, 0.0f},
            {0.707107f, 0.707107f},
    };

    constexpr size_t pointCount = sizeof(sample) / sizeof(sample[0]);
    static_assert(pointCount == sizeof(expectedWarpSamples)/sizeof(expectedWarpSamples[0]));

    for (size_t index = 0; index < pointCount; ++index) {
        auto warpped_sample = warpUniformSquareToConcentricDisk(sample[index]);

        EXPECT_THAT(warpped_sample.x(), testing::FloatEq(expectedWarpSamples[index].x()));
        EXPECT_THAT(warpped_sample.y(), testing::FloatEq(expectedWarpSamples[index].y()));
    }
}

TEST(Warping, GivenARotationAngle_WhenComputingRotatedUnitVector_ThenExpectProperRotatedUnitVector) {
    float rotationAngle[] = {
            0.0f,
            45.0f,
            90.0f,
            135.0f,
            270.0f
    };

    Vector2d expectedRotatedUnitVector[] = {
            {1.0f, 0.0f},
            {0.707107f, 0.707107f},
            {0.0f, 1.0f},
            {-0.707107f, 0.707107f},
            {0.0f, -1.0f},
    };

    constexpr size_t Count = sizeof(expectedRotatedUnitVector) / sizeof(expectedRotatedUnitVector[0]);
    static_assert(Count == sizeof(rotationAngle)/sizeof(rotationAngle[0]));

    for (size_t i = 0; i < Count; ++i) {
        auto v = rotatedUnitVector(degreeToRadian(rotationAngle[i]));
        EXPECT_NEAR(v.x(), expectedRotatedUnitVector[i].x(), 0.001f);
        EXPECT_NEAR(v.y(), expectedRotatedUnitVector[i].y(), 0.001f);
    }
}

TEST(Warping, uniformSampleHalfCircle) {
    auto v = uniformSampleHalfCircle(0.5);

    EXPECT_NEAR(v.x(), 0.0f, 0.001f);
    EXPECT_NEAR(v.y(), 1.0f, 0.001f);
}

TEST(Warping, sampleHalfCircle) {
    auto v = sampleHalfCircle(Point2f{0.5f, 0.5f});

    EXPECT_NEAR(v.x(), 0.0f, 0.001f);
    EXPECT_NEAR(v.y(), 0.0f, 0.001f);
}