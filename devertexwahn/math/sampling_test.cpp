/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/sampler.hpp"
#include "math/sampling.hpp"
using namespace de_vertexwahn;

#include "hypothesis.h"

#include <array>

#include "gmock/gmock.h"

TEST(Warping, warp_uniform_square_to_concentric_disk) {
    Point2f sample[] = {
            {0.f,  0.f},
            {0.5f, 0.5f},
            {1.f,  1.f},
    };

    Point2f expectedWarpSamples[] = {
            {-0.707107f, -0.707107f},
            {0.f,        0.f},
            {0.707107f,  0.707107f},
    };

    constexpr size_t point_count = sizeof(sample) / sizeof(sample[0]);
    static_assert(point_count == sizeof(expectedWarpSamples) / sizeof(expectedWarpSamples[0]));

    for (size_t index = 0; index < point_count; ++index) {
        auto warpped_sample = warp_uniform_square_to_concentric_disk(sample[index]);

        EXPECT_THAT(warpped_sample.x(), testing::FloatEq(expectedWarpSamples[index].x()));
        EXPECT_THAT(warpped_sample.y(), testing::FloatEq(expectedWarpSamples[index].y()));
    }
}

TEST(Warping, GivenArotation_angle_WhenComputingRotatedUnitVector_ThenExpectProperRotatedUnitVector) {
    float rotation_angle[] = {
            0.f,
            45.f,
            90.f,
            135.f,
            270.f
    };

    Vector2d expected_rotated_unit_vector[] = {
            {1.f,        0.f},
            {0.707107f,  0.707107f},
            {0.f,        1.f},
            {-0.707107f, 0.707107f},
            {0.f,        -1.f},
    };

    constexpr size_t Count = sizeof(expected_rotated_unit_vector) / sizeof(expected_rotated_unit_vector[0]);
    static_assert(Count == sizeof(rotation_angle) / sizeof(rotation_angle[0]));

    for (size_t i = 0; i < Count; ++i) {
        auto v = rotated_unit_vector(degree_to_radian(rotation_angle[i]));
        EXPECT_NEAR(v.x(), expected_rotated_unit_vector[i].x(), 0.001f);
        EXPECT_NEAR(v.y(), expected_rotated_unit_vector[i].y(), 0.001f);
    }
}

TEST(Warping, uniform_sample_half_circle) {
    auto v = uniform_sample_half_circle(.5);

    EXPECT_NEAR(v.x(), 0.f, 0.001f);
    EXPECT_NEAR(v.y(), 1.f, 0.001f);
}

TEST(Warping, sample_half_circle) {
    auto v = sample_half_circle(Point2f{0.5f, 0.5f});

    EXPECT_NEAR(v.x(), 0.f, 0.001f);
    EXPECT_NEAR(v.y(), 0.f, 0.001f);
}

TEST(Warping, XiQuadartTest_warpUniformSquareToConcentricDisk) {
    // Arrange
    PropertySet ps;
    ps.add_property("deterministic_seed", true);
    IndependentSampler sampler{ps};

    constexpr int resolution_x = 51;
    constexpr int resolution_y = 51;
    constexpr int resolution = resolution_x * resolution_y;
    constexpr int sample_count = 1000 * resolution;

    std::array<double, resolution> observed_frequencies{0.0};

    // Act
    for (size_t i = 0; i < sample_count; ++i) {
        auto sample = warp_uniform_square_to_concentric_disk(sampler.next_2d());
        int x = std::floor((sample.x() * 0.5 + 0.5) * resolution_x);
        int y = std::floor((sample.y() * 0.5 + 0.5) * resolution_y);
        int bucketIndex = y * resolution_x + x;
        observed_frequencies[bucketIndex] += 1.0;
    }

    std::array<double, resolution> expectedFrequencies{0.0};

    auto f = [&](double y, double x) -> double {
        x = x * 2 - 1;
        y = y * 2 - 1;
        return warp_uniform_square_to_concentric_disk_pdf(Point2d(x, y));
    };

    double scale = sample_count * 4.0;

    for (int x = 0; x < resolution_x; ++x) {
        for (int y = 0; y < resolution_y; ++y) {
            double x0 = (double) x / resolution_x;
            double y0 = (double) y / resolution_y;
            double x1 = (x + 1.0) / resolution_x;
            double y1 = (y + 1.0) / resolution_y;

            expectedFrequencies[y * resolution_x + x] = hypothesis::adaptiveSimpson2D(
                    f, x0, y0, x1, y1) * scale;

            if (expectedFrequencies[y * resolution_x + x] < 0.0)
                throw std::runtime_error("Cumulative distribution function must be positive");
        }
    }

    // Assert
    const int min_exp_frequency = 5;
    const float significance_level = 0.01f;

    std::pair<bool, std::string> result = hypothesis::chi2_test(resolution_y * resolution_x,
                                                                observed_frequencies.data(),
                                                                expectedFrequencies.data(),
                                                                sample_count,
                                                                min_exp_frequency,
                                                                significance_level,
                                                                1);

    EXPECT_THAT(result.second, testing::HasSubstr("(d.o.f. = 2125)"));
    EXPECT_THAT(result.second, testing::HasSubstr("Accepted the null hypothesis"));
}

