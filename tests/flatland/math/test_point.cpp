/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/math/point.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(Point2f, Point2f_ctor) {
    Point2f p{1.0f, 2.0f};

    EXPECT_THAT(p.x(), 1.0f);
    EXPECT_THAT(p.y(), 2.0f);
}
/*
TEST(Point2f, Point2f_array_ctor) {
	float pa[2] = {1.0f, 2.0f};
	Point2f p(pa);

	EXPECT_THAT(p.x(), 1.0f);
	EXPECT_THAT(p.y(), 2.0f);
}
*/
