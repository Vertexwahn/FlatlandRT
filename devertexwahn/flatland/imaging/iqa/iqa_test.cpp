/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/imaging/iqa/iqa.h"

#include "include/gmock/gmock.h"

using namespace flatland;

TEST(IQA, GivenIdenticalImages_WhenComparing_ExpectEqual) {
    // Arrange
    Image3f a{100, 100};
    Image3f b{100, 100};

    // Act
    EXPECT_THAT(are_equal(a, b), true);
}

TEST(IQA, GivenDifferentImages_WhenComparing_ExpectNotEqual) {
    Image3f a{100, 100};
    Image3f b{100, 100};
    b.set_pixel(0,0, Color3f{1.f, .5f, .25f});
    EXPECT_THAT(are_equal(a, b), false);
}

TEST(IQA, mse) {
    // Arrange
    Image3f a{4, 4};
    Image3f b{4, 4};

    for(int y = 0; y < 4; ++y) {
        for(int x = 0; x < 4; ++x) {
            a.set_pixel(x, y, Color3f{1.f, 1.f, 1.f});
            b.set_pixel(x, y, Color3f{0.f, 0.f, 0.f});
        }
    }

    // Act
    auto result = mean_squared_error(a, b);

    // Assert
    EXPECT_THAT(result, 1.f);
}