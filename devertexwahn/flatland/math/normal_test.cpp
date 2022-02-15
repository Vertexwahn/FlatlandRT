/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/math/normal.h"

#include "gmock/gmock.h"

using namespace flatland;

TEST(Normal2f, Normal2f_ctor) {
    Normal2f n{1.f, 0.f};

    EXPECT_THAT(n.x(), 1.f);
    EXPECT_THAT(n.y(), 0.f);
}

TEST(Normal2f, Normal3f_ctor) {
    Normal3f n{0.f, 1.f, 0.f};

    EXPECT_THAT(n.x(), 0.f);
    EXPECT_THAT(n.y(),1.f);
    EXPECT_THAT(n.z(), 0.f);
}
/*
TEST(Normal2f, Normal2f_array_ctor) {
	float na[2] = {1.0, 2.0};
	Normal2f n{na};

	EXPECT_TRUE(n.x() == 1.f);
	EXPECT_TRUE(n.y() == 2.f);
}
*/