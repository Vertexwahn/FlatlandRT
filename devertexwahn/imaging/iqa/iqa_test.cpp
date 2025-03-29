/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/iqa/iqa.hpp"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(IQA, GivenIdenticalImages_WhenComparing_ExpectEqual) {
    // Arrange
    Image3f a{100, 100};
    Image3f b{100, 100};

    // Act
    EXPECT_THAT(are_equal(a, b), true);
}

TEST(IQA_Image3b, GivenIdenticalImages_WhenComparing_ExpectEqual) {
    // Arrange
    Image3b a{100, 100};
    Image3b b{100, 100};

    // Act
    EXPECT_THAT(are_equal(a, b), true);
}

TEST(IQA, GivenDifferentImages3f_WhenComparing_ExpectNotEqual) {
    Image3f a{100, 100};
    Image3f b{100, 100};
    b.set_pixel(0, 0, ColorRGB3f{1.f, .5f, .25f});
    EXPECT_THAT(are_equal(a, b), false);
}

TEST(IQA, GivenDifferentImages3b_WhenComparing_ExpectNotEqual) {
    Image3b a{100, 100};
    Image3b b{100, 100};
    b.set_pixel(0, 0, ColorRGB3b{255, 128, 64});
    EXPECT_THAT(are_equal(a, b), false);
}

TEST(IQA, mse) {
    // Arrange
    Image3f a{4, 4};
    Image3f b{4, 4};

    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            a.set_pixel(x, y, ColorRGB3f{1.f, 1.f, 1.f});
            b.set_pixel(x, y, ColorRGB3f{0.f, 0.f, 0.f});
        }
    }

    // Act
    auto result = mean_squared_error(a, b);

    // Assert
    EXPECT_THAT(result, 1.f);
}

TEST(IQA, mean_squared_error) {
    ReferenceCounted<Image3f> a = make_reference_counted<Image3f>(4, 4);
    ReferenceCounted<Image3f> b = make_reference_counted<Image3f>(4, 4);

    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            a->set_pixel(x, y, ColorRGB3f{1.f, 1.f, 1.f});
            b->set_pixel(x, y, ColorRGB3f{1.f, 1.f, 1.f});
        }
    }

    EXPECT_THAT(mean_squared_error(a, b), 0.f);
}
