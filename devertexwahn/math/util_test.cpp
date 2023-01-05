/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "math/util.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(DegreeToRadian, When_90Degrees_Then_1DOT5708Radians) {
    auto value_in_degree = 90.0;
    auto value_in_radian = degree_to_radian(value_in_degree);

    EXPECT_THAT(value_in_radian, testing::DoubleNear(1.5708, 0.0001));
}

TEST(Util, mid_point_with_float) {
    // Arrange
    float p1 = 0;
    float p2 = 1;

    // Act
    auto result = mid_point(p1, p2);

    // Assert
    EXPECT_THAT(result, .5f);
}

TEST(Util, min_3_args) {
    EXPECT_THAT(min(1, 2, 3), 1);
    EXPECT_THAT(min(2, 1, 3), 1);
    EXPECT_THAT(min(2, 3, 1), 1);
    EXPECT_THAT(min(3, 2, 1), 1);
    EXPECT_THAT(min(3.f, 2.f, 1.f), 1.f);
    EXPECT_THAT(min(3.0, 2.0, 1.0), 1.0);
}

TEST(Util, max_3_args) {
    EXPECT_THAT(max(1, 2, 3), 3);
    EXPECT_THAT(max(2, 1, 3), 3);
    EXPECT_THAT(max(2, 3, 1), 3);
    EXPECT_THAT(max(3, 2, 1), 3);
    EXPECT_THAT(max(3.f, 2.f, 1.f), 3.f);
    EXPECT_THAT(max(3.0, 2.0, 1.0), 3.0);
}
