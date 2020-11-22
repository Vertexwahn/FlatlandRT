/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/


#include "flatland/core/sampler.h"
#include "flatland/math/warping.h"

#include <hypothesis.h>
#include <gmock/gmock.h>
#include <array>

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

TEST(Warping, XiQuadartTest_warpUniformSquareToConcentricDisk) {
    Sampler2f sampler{false};

    constexpr int resolution_x = 51;
    constexpr int resolution_y = 51;
    constexpr int resolution = resolution_x * resolution_y;
    constexpr int sampleCount = 1000 * resolution;

    std::array<double, resolution> observedFrequencies{ 0.0 };

    for (size_t i = 0; i < sampleCount; ++i) {
        auto sample = warpUniformSquareToConcentricDisk(sampler.next2d());
        int x = std::floor((sample.x() * 0.5 + 0.5) * resolution_x);
        int y = std::floor((sample.y() * 0.5 + 0.5) * resolution_y);
        int bucketIndex = y * resolution_x + x;
        observedFrequencies[bucketIndex] += 1.0;
    }

    std::array<double, resolution> expectedFrequencies{ 0.0 };

    auto f = [&](double y, double x) -> double {
        x = x * 2 - 1;
        y = y * 2 - 1;
        return warpUniformSquareToConcentricDiskPdf(Point2d(x,y));
    };

    double scale = sampleCount * 4.0;

    for (int x = 0; x < resolution_x; ++x) {
        for (int y = 0; y < resolution_y; ++y) {
            double x0 = (double)x / resolution_x;
            double y0 = (double)y / resolution_y;
            double x1 = (x + 1.0) / resolution_x;
            double y1 = (y + 1.0) / resolution_y;

            expectedFrequencies[y * resolution_x + x] = hypothesis::adaptiveSimpson2D(
                    f, x0, y0, x1, y1) * scale;

            if (expectedFrequencies[y * resolution_x + x] < 0.0)
                throw std::runtime_error("Cumulative distribution function must be positive");
        }
    }

    const int minExpFrequency = 5;
    const float significanceLevel = 0.01f;

    std::pair<bool, std::string> result = hypothesis::chi2_test(resolution_y * resolution_x, observedFrequencies.data(),
        expectedFrequencies.data(), sampleCount,
        minExpFrequency, significanceLevel, 1);

    EXPECT_THAT(result.second, testing::HasSubstr("(d.o.f. = 2125)"));
    EXPECT_THAT(result.second, testing::HasSubstr("Accepted the null hypothesis"));
}