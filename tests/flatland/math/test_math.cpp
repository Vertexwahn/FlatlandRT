/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/math/util.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(DegreeToRadian, When_90Degrees_Then_1DOT5708Radians) {
    auto valueInDegree = 90.0;
    auto valueInRadian = degreeToRadian(valueInDegree);

    EXPECT_THAT(valueInRadian, ::testing::DoubleNear(1.5708, 0.0001));
}