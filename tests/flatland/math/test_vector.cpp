/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/math/vector.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(Vector2i, Vector2i_ctor) {
    Vector2f v{1, 2};

    EXPECT_THAT(v.x(), 1);
    EXPECT_THAT(v.y(), 2);
}

TEST(Vector2f, Vector2f_ctor) {
    Vector2f v{1.0f, 2.0f};

    EXPECT_THAT(v.x(), 1.0f);
    EXPECT_THAT(v.y(), 2.0f);
}

TEST(Vector3f, Vector3f_ctor) {
    Vector3f v{1.0f, 2.0f, 3.0f};

    EXPECT_THAT(v.x(), 1.0f);
    EXPECT_THAT(v.y(), 2.0f);
    EXPECT_THAT(v.z(), 3.0f);
}

TEST(Vector3d, Vector3d_ctor) {
    Vector3f v{1.0, 2.0, 3.0};

    EXPECT_THAT(v.x(), 1.0);
    EXPECT_THAT(v.y(), 2.0);
    EXPECT_THAT(v.z(), 3.0);
}

TEST(Vector4f, Vector4f_ctor) {
    Vector4f v{1.0f, 2.0f, 3.0f, 0.0f};

    EXPECT_THAT(v.x(), 1.0f);
    EXPECT_THAT(v.y(), 2.0f);
    EXPECT_THAT(v.z(), 3.0f);
    EXPECT_THAT(v.w(), 0.0f);
}
/*
TEST(Vector2f, Vector2f_array_ctor) {
    float va[2] = {1.0f, 2.0f};
    Vector2f v{va};

    EXPECT_THAT(v.x(), 1.0f);
    EXPECT_THAT(v.y(), 2.0f);
}
*/

TEST(Vector2f, Vector2f_compare) {
    Vector2f a{1.0f, 2.0f};
    Vector2f b{1.0f, 2.0f};

    EXPECT_TRUE(a == b);
}

TEST(Vector2f, Vector2f_compare2) {
    Vector2f a{1.0f, 2.0f};
    Vector2f b{0.0f, 2.0f};

    EXPECT_TRUE(a != b);
}

TEST(Vector2f, Vector2f_negate) {
    Vector2f a{1.0f, 2.0f};

    Vector2f b = -a;

    EXPECT_THAT(b.x(), -a.x());
    EXPECT_THAT(b.y(), -a.y());
}

TEST(Vector2f, Vector2f_add) {
	Vector2f a{1.0f, 2.0f};
	Vector2f b{1.0f, 2.0f};

	Vector2f c = a + b;

    EXPECT_THAT(c.x(), 2.0f);
    EXPECT_THAT(c.y(), 4.0f);
}

TEST(Vector2f, Vector2f_subtract) {
	Vector2f a{1.0f, 2.0f};
	Vector2f b{1.0f, 2.0f};

	Vector2f c = a - b;

    EXPECT_THAT(c.x(), 0.0f);
    EXPECT_THAT(c.y(), 0.0f);
}

TEST(Vector2f, Vector2f_normalize) {
	Vector2f v{1234.0f, 0.0f};
	v.normalize();

    EXPECT_THAT(v.x(), 1.0f);
    EXPECT_THAT(v.y(), 0.0f);
}

TEST(Vector2f, Vector2f_norm) {
	Vector2f v{1234.0f, 0.0f};

    EXPECT_THAT(v.norm(), 1234.0f);
}

TEST(Vector2f, Vector2f_scalar_multiplication) {
	Vector2f a{1.0f, 2.0f};

	Vector2f b = 4.0f * a;

    EXPECT_THAT(b.x(), 4.0f);
    EXPECT_THAT(b.y(), 8.0f);
}

TEST(Vector2f, Vector2f_scalar_multiplication2) {
    Vector2f a{1.0f, 2.0f};

    Vector2f b = a * 4.0f;

    EXPECT_THAT(b.x(), 4.0f);
    EXPECT_THAT(b.y(), 8.0f);
}

TEST(Vecto2f, Vector2f_WhenOrthogonalUnitVectors_ExpectZeroAsScalarProduct) {
    Vector2f a{1.0f, 0.0f};
    Vector2f b{0.0f, 1.0f};

    auto s = a.dot(b);

    EXPECT_THAT(s, 0.0f);
}

TEST(Vecto2f, WhenIdenticalUnitVectors_ExpectOneAsScalarProduct) {
    Vector2f a{1.0f, 0.0f};
    Vector2f b{1.0f, 0.0f};

    auto s = a.dot(b);

    EXPECT_THAT(s, 1.0f);
}

TEST(Vecto2f, WhenOpositeFacingUnitVectors_ExpectMinusOneSclarProduct) {
    Vector2f a{1.0f, 0.0f};
    Vector2f b{-1.0f, 0.0f};

    auto s = a.dot(b);

    EXPECT_THAT(s, -1.0f);
}

TEST(Vector2f, crossProduct2) {
    Vector2f a{4.0f, 7.0f};
    Vector2f b{3.0f, 1.0f};

    EXPECT_THAT(crossProduct2(a,b), -17.0f);
}