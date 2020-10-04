/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/core/color.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(Color3f, Color3f_ctor) {
    Color3f c{1.0f, 2.0f, 3.0f};

    EXPECT_THAT(c.red(), 1.0f);
    EXPECT_THAT(c.green(), 2.0f);
    EXPECT_THAT(c.blue(), 3.0f);
}

TEST(Color3f, Color3f_data) {
    Color3f c{1.0f, 2.0f, 3.0f};

    EXPECT_THAT(c.data()[0], 1.0f);
    EXPECT_THAT(c.data()[1], 2.0f);
    EXPECT_THAT(c.data()[2], 3.0f);
}

TEST(Color3f, GivenSingleColor_WhenComparedToItself_ThenEqual) {
    Color3f c{1.0f, 2.0f, 3.0f};

    bool equal = (c == c);

    EXPECT_TRUE(equal);
}

TEST(Color3f, GivenIdenticalColors_WhenCompared_ThenEqual) {
    Color3f c1{1.0f, 2.0f, 3.0f};
    Color3f c2{1.0f, 2.0f, 3.0f};

    bool equal = (c1 == c2);

    EXPECT_TRUE(equal);
}

TEST(Color3f, GivenDifferentColors_WhenCompared_ThenUnEqual) {
    Color3f c1{1.0f, 2.0f, 3.0f};
    Color3f c2{3.0f, 2.0f, 1.0f};

    bool unequal = !(c1 == c2);

    EXPECT_TRUE(unequal);
}

TEST(Color3f, assignment) {
    Color3f c1{1.0f, 2.0f, 3.0f};
    Color3f c2;

    c2 = c1;

    EXPECT_THAT(c1.red(), c2.red());
    EXPECT_THAT(c1.green(), c2.green());
    EXPECT_THAT(c1.blue(), c2.blue());
}

// LLVM complains about this test
/*
TEST(Color3f, self_assignment) {
    Color3f c1{1.0f, 2.0f, 3.0f};
    Color3f c2;

    c2 = c1;
    c1 = c1;

    EXPECT_THAT(c1.red(), c2.red());
    EXPECT_THAT(c1.green(), c2.green());
    EXPECT_THAT(c1.blue(), c2.blue());
}
*/

TEST(Color3f, self_getSize) {
    Color3f c;

    EXPECT_THAT(c.size(), 3u);
}