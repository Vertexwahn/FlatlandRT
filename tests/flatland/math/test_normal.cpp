/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/math/normal.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(Normal2f, Normal2f_ctor) {
    Normal2f n{1.0f, 0.0f};

    EXPECT_THAT(n.x(), 1.0f);
    EXPECT_THAT(n.y(), 0.0f);
}

TEST(Normal2f, Normal3f_ctor) {
    Normal3f n{0.0f, 1.0f, 0.0f};

    EXPECT_THAT(n.x(), 0.0f);
    EXPECT_THAT(n.y(),1.0f);
    EXPECT_THAT(n.z(), 0.0f);
}
/*
TEST(Normal2f, Normal2f_array_ctor) {
	float na[2] = {1.0, 2.0};
	Normal2f n{na};

	EXPECT_TRUE(n.x() == 1.0f);
	EXPECT_TRUE(n.y() == 2.0f);
}
*/