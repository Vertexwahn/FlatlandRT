/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "math/util.hpp"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(util, lerp) {
    EXPECT_THAT(lerp(0.f, 1.f, .5f), .5f);
}

TEST(util, sqr) {
    EXPECT_THAT(sqr(1), 1);
    EXPECT_THAT(sqr(2), 4);
    EXPECT_THAT(sqr(3), 9);
}

TEST(util, clamp) {
    EXPECT_THAT(clamp(-1, 0, 10), 0);
    EXPECT_THAT(clamp(1, 0, 10), 1);
    EXPECT_THAT(clamp(11, 0, 10), 10);
}

TEST(DegreeToRadian, When_90Degrees_Then_1DOT5708Radians) {
    auto value_in_degree = 90.0;
    auto value_in_radian = degree_to_radian(value_in_degree);

    EXPECT_THAT(value_in_radian, testing::DoubleNear(1.5708, 0.0001));
}

TEST(util, mid_point_with_float) {
    // Arrange
    float p1 = 0;
    float p2 = 1;

    // Act
    auto result = mid_point(p1, p2);

    // Assert
    EXPECT_THAT(result, .5f);
}

TEST(util, min_3_args) {
    EXPECT_THAT(min(1, 2, 3), 1);
    EXPECT_THAT(min(2, 1, 3), 1);
    EXPECT_THAT(min(2, 3, 1), 1);
    EXPECT_THAT(min(3, 2, 1), 1);
    EXPECT_THAT(min(3.f, 2.f, 1.f), 1.f);
    EXPECT_THAT(min(3.0, 2.0, 1.0), 1.0);
}

TEST(util, max_3_args) {
    EXPECT_THAT(max(1, 2, 3), 3);
    EXPECT_THAT(max(2, 1, 3), 3);
    EXPECT_THAT(max(2, 3, 1), 3);
    EXPECT_THAT(max(3, 2, 1), 3);
    EXPECT_THAT(max(3.f, 2.f, 1.f), 3.f);
    EXPECT_THAT(max(3.0, 2.0, 1.0), 3.0);
}

TEST(util, next_pow2) {
    EXPECT_THAT(next_pow2(1u), 1);
    EXPECT_THAT(next_pow2(2u), 2);
    EXPECT_THAT(next_pow2(3u), 4);
    EXPECT_THAT(next_pow2(4u), 4);
    EXPECT_THAT(next_pow2(5u), 8);
    EXPECT_THAT(next_pow2(6u), 8);
    EXPECT_THAT(next_pow2(7u), 8);
    EXPECT_THAT(next_pow2(8u), 8);
    EXPECT_THAT(next_pow2(9u), 16);
    EXPECT_THAT(next_pow2(10u), 16);
    EXPECT_THAT(next_pow2(100u), 128);
    EXPECT_THAT(next_pow2(1000u), 1024);
}
