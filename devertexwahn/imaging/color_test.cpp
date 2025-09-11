/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/color.hpp"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(Color3f, Color3f_ctor) {
    ColorRGB3f c{1.f, 2.f, 3.f};

    EXPECT_THAT(c.red(), 1.f);
    EXPECT_THAT(c.green(), 2.f);
    EXPECT_THAT(c.blue(), 3.f);
}

TEST(Color3f, Color3f_data) {
    ColorRGB3f c{1.f, 2.f, 3.f};

    EXPECT_THAT(c.data()[0], 1.f);
    EXPECT_THAT(c.data()[1], 2.f);
    EXPECT_THAT(c.data()[2], 3.f);
}

TEST(Color3f, GivenSingleColor_WhenComparedToItself_ThenEqual) {
    ColorRGB3f c{1.f, 2.f, 3.f};

    bool equal = (c == c);

    EXPECT_TRUE(equal);
}

TEST(Color3f, GivenIdenticalColors_WhenCompared_ThenEqual) {
    ColorRGB3f c1{1.f, 2.f, 3.f};
    ColorRGB3f c2{1.f, 2.f, 3.f};

    bool equal = (c1 == c2);

    EXPECT_TRUE(equal);
}

TEST(Color3f, GivenDifferentColors_WhenCompared_ThenUnEqual) {
    ColorRGB3f c1{1.f, 2.f, 3.f};
    ColorRGB3f c2{3.f, 2.f, 1.f};

    bool unequal = !(c1 == c2);

    EXPECT_TRUE(unequal);
}

TEST(Color3f, assignment) {
    ColorRGB3f c1{1.f, 2.f, 3.f};
    ColorRGB3f c2;

    c2 = c1;

    EXPECT_THAT(c1.red(), c2.red());
    EXPECT_THAT(c1.green(), c2.green());
    EXPECT_THAT(c1.blue(), c2.blue());
}

TEST(Color3f, ostream) {
    ColorRGB3f c{1.f, 2.f, 3.f};
    std::stringstream ss;
    ss << c;

    EXPECT_THAT(ss.str(), testing::HasSubstr("1\n2\n3"));
}

// LLVM complains about this test
/*
TEST(Color3f, self_assignment) {
    Color3f c1{1.f, 2.f, 3.f};
    Color3f c2;

    c2 = c1;
    c1 = c1;

    EXPECT_THAT(c1.red(), c2.red());
    EXPECT_THAT(c1.green(), c2.green());
    EXPECT_THAT(c1.blue(), c2.blue());
}
*/

TEST(Color3f, self_getSize) {
    ColorRGB3f c;

    EXPECT_THAT(c.size(), 3u);
}

TEST(Color3f, clamp) {
    ColorRGB3f c1{2.0f, -1.0f, 1.0f};
    c1.clamp();

    EXPECT_THAT(c1.red(), 1.0f);
    EXPECT_THAT(c1.green(), 0.0f);
    EXPECT_THAT(c1.blue(), 1.0f);
}

TEST(Color3f, clamp_negative) {
    ColorRGB3f c1{2.0f, -1.0f, 1.0f};
    c1.clamp_negative();

    EXPECT_THAT(c1.red(), 2.0f);
    EXPECT_THAT(c1.green(), 0.0f);
    EXPECT_THAT(c1.blue(), 1.0f);
}

TEST(Color3f, hasnans) {
    ColorRGB3f a{0.f, 0.f, 0.f};
    ColorRGB3f b{0.f, NAN, 0.f};
    EXPECT_THAT(a.has_nans(), false);
    EXPECT_THAT(b.has_nans(), true);
}

TEST(Color3f, multiply) {
    ColorRGB3f a{.5f};
    ColorRGB3f b{.5f};

    ColorRGB3f c = a.cwiseProduct(b);

    EXPECT_THAT(c.red(), 0.25f);
    EXPECT_THAT(c.green(), 0.25f);
    EXPECT_THAT(c.blue(), 0.25f);

    ColorRGB3f d = a * b;

    EXPECT_THAT(d.red(), 0.25f);
    EXPECT_THAT(d.green(), 0.25f);
    EXPECT_THAT(d.blue(), 0.25f);
}

TEST(Color3f, to_string) {
    ColorRGB3f c{.5f, .6f, .7f};
    EXPECT_THAT(c.to_string(), "0.5, 0.6, 0.7");
}

TEST(Color3f, hot_to_cold_color_ramp) {
    ColorRGB3f cold = hot_to_cold_color_ramp(.0f, .0f, 1.f);

    EXPECT_THAT(cold.red(), 0.0f);
    EXPECT_THAT(cold.green(), 0.0f);
    EXPECT_THAT(cold.blue(), 1.0f);

    ColorRGB3f hot = hot_to_cold_color_ramp(1.f, .0f, 1.f);

    EXPECT_THAT(hot.red(), 1.0f);
    EXPECT_THAT(hot.green(), 0.0f);
    EXPECT_THAT(hot.blue(), 0.0f);
}
