/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "math/point.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(Point2f, Point2f_ctor) {
    Point2f p{1.f, 2.f};

    EXPECT_THAT(p.x(), 1.f);
    EXPECT_THAT(p.y(), 2.f);
}

TEST(Point4f, swizzle_xyz) {
    Point4f p{1.f, 2.f, 3.f, 4.f};
    EXPECT_THAT(p.xyz(), (Point3f{1.f, 2.f, 3.f}));
}

TEST(Point4f, swizzle_zy) {
    Point4f p{1.f, 2.f, 3.f, 4.f};
    EXPECT_THAT(p.zy(), (Point2f{3.f, 2.f}));
}

TEST(Point3i, Point2i_ctor) {
    Point3i p{1, 2, 3};

    EXPECT_THAT(p.x(), 1);
    EXPECT_THAT(p.y(), 2);
    EXPECT_THAT(p.z(), 3);
}

/*
TEST(Point2f, Point2f_array_ctor) {
	float pa[2] = {1.f, 2.f};
	Point2f p(pa);

	EXPECT_THAT(p.x(), 1.f);
	EXPECT_THAT(p.y(), 2.f);
}
*/

TEST(Point2f, hasnans) {
    Point2f a{0.f, 0.0f};
    Point2f b{0.f, NAN};
    EXPECT_THAT(a.has_nans(), false);
    EXPECT_THAT(b.has_nans(), true);
}
