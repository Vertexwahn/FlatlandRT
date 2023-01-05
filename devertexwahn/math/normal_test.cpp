/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "math/normal.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(Normal2f, Normal2f_ctor) {
    Normal2f n{1.f, 0.f};

    EXPECT_THAT(n.x(), 1.f);
    EXPECT_THAT(n.y(), 0.f);
}

TEST(Normal2f, Normal3f_ctor) {
    Normal3f n{0.f, 1.f, 0.f};

    EXPECT_THAT(n.x(), 0.f);
    EXPECT_THAT(n.y(), 1.f);
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

TEST(Normal2f, hasnans) {
    Normal2f a{0.f, 0.0f};
    Normal2f b{0.f, NAN};
    EXPECT_THAT(a.has_nans(), false);
    EXPECT_THAT(b.has_nans(), true);
}
