/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "math/vector.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(Vector2i, Vector2i_ctor) {
    Vector2f v{1.f, 2.f};

    EXPECT_THAT(v.x(), 1.f);
    EXPECT_THAT(v.y(), 2.f);
}

TEST(Vector2f, Vector2f_ctor) {
    Vector2f v{1.f, 2.f};

    EXPECT_THAT(v.x(), 1.f);
    EXPECT_THAT(v.y(), 2.f);
}

TEST(Vector2f, Vector2f_CtorWithSingleArgument) {
    Vector2f v1{0.f};
    EXPECT_THAT(v1.x(), 0.f);
    EXPECT_THAT(v1.y(), 0.f);

    Vector2f v2{2.f};
    EXPECT_THAT(v2.x(), 2.f);
    EXPECT_THAT(v2.y(), 2.f);
}

TEST(Vector3f, Vector3f_ctor) {
    Vector3f v{1.f, 2.f, 3.f};

    EXPECT_THAT(v.x(), 1.f);
    EXPECT_THAT(v.y(), 2.f);
    EXPECT_THAT(v.z(), 3.f);
}

TEST(Vector3d, Vector3d_ctor) {
    Vector3d v{1.0, 2.0, 3.0};

    EXPECT_THAT(v.x(), 1.0);
    EXPECT_THAT(v.y(), 2.0);
    EXPECT_THAT(v.z(), 3.0);
}

TEST(Vector4f, Vector4f_ctor) {
    Vector4f v{1.f, 2.f, 3.f, 0.f};

    EXPECT_THAT(v.x(), 1.f);
    EXPECT_THAT(v.y(), 2.f);
    EXPECT_THAT(v.z(), 3.f);
    EXPECT_THAT(v.w(), 0.f);
}
/*
TEST(Vector2f, Vector2f_array_ctor) {
    float va[2] = {1.f, 2.f};
    Vector2f v{va};

    EXPECT_THAT(v.x(), 1.f);
    EXPECT_THAT(v.y(), 2.f);
}
*/

TEST(Vector2f, Vector2f_compare) {
    Vector2f a{1.f, 2.f};
    Vector2f b{1.f, 2.f};

    EXPECT_TRUE(a == b);
}

TEST(Vector2f, Vector2f_compare2) {
    Vector2f a{1.f, 2.f};
    Vector2f b{0.f, 2.f};

    EXPECT_TRUE(a != b);
}

TEST(Vector2f, Vector2f_negate) {
    Vector2f a{1.f, 2.f};

    Vector2f b = -a;

    EXPECT_THAT(b.x(), -a.x());
    EXPECT_THAT(b.y(), -a.y());
}

TEST(Vector2f, Vector2f_add) {
    Vector2f a{1.f, 2.f};
    Vector2f b{1.f, 2.f};

    Vector2f c = a + b;

    EXPECT_THAT(c.x(), 2.f);
    EXPECT_THAT(c.y(), 4.f);
}

TEST(Vector2f, Vector2f_subtract) {
    Vector2f a{1.f, 2.f};
    Vector2f b{1.f, 2.f};

    Vector2f c = a - b;

    EXPECT_THAT(c.x(), 0.f);
    EXPECT_THAT(c.y(), 0.f);
}

TEST(Vector2f, Vector2f_normalize) {
    Vector2f v{1234.f, 0.f};
    v.normalize();

    EXPECT_THAT(v.x(), 1.f);
    EXPECT_THAT(v.y(), 0.f);
}

TEST(Vector2f, Vector2f_norm) {
    Vector2f v{1234.f, 0.f};

    EXPECT_THAT(v.norm(), 1234.f);
}

TEST(Vector2f, Vector2f_scalar_multiplication) {
    Vector2f a{1.f, 2.f};

    Vector2f b = 4.f * a;

    EXPECT_THAT(b.x(), 4.f);
    EXPECT_THAT(b.y(), 8.f);
}

TEST(Vector2f, Vector2f_scalar_multiplication2) {
    Vector2f a{1.f, 2.f};

    Vector2f b = a * 4.f;

    EXPECT_THAT(b.x(), 4.f);
    EXPECT_THAT(b.y(), 8.f);
}

TEST(Vecto2f, Vector2f_WhenOrthogonalUnitVectors_ExpectZeroAsScalarProduct) {
    Vector2f a{1.f, 0.f};
    Vector2f b{0.f, 1.f};

    auto s = a.dot(b);

    EXPECT_THAT(s, 0.f);
}

TEST(Vecto2f, WhenIdenticalUnitVectors_ExpectOneAsScalarProduct) {
    Vector2f a{1.f, 0.f};
    Vector2f b{1.f, 0.f};

    auto s = a.dot(b);

    EXPECT_THAT(s, 1.f);
}

TEST(Vecto2f, WhenOpositeFacingUnitVectors_ExpectMinusOneSclarProduct) {
    Vector2f a{1.f, 0.f};
    Vector2f b{-1.f, 0.f};

    auto s = a.dot(b);

    EXPECT_THAT(s, -1.f);
}

TEST(Vector2f, cross_product_2) {
    Vector2f a{4.f, 7.f};
    Vector2f b{3.f, 1.f};

    EXPECT_THAT(cross_product_2(a, b), -17.f);
}

TEST(Vector2f, hasnans) {
    Vector2f a{0.f, 0.0f};
    Vector2f b{0.f, NAN};
    EXPECT_THAT(a.has_nans(), false);
    EXPECT_THAT(b.has_nans(), true);
}
