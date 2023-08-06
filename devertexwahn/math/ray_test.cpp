/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "math/ray.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(Ray2f, GivenDirectionOriginMaxMinT_WhenRayIsInitialized_ThenInitzalizedRayValues) {
    Point2f origin{0.f, 0.f};
    Vector2f direction{1.f, 0.f};

    Ray2f r(origin, direction, 1.f, 10.f);

    EXPECT_THAT(r.origin.x(), 0.f);
    EXPECT_THAT(r.origin.y(), 0.f);
    EXPECT_THAT(r.direction.x(), 1.f);
    EXPECT_THAT(r.direction.y(), 0.f);
    EXPECT_THAT(r.min_t, 1.f);
    EXPECT_THAT(r.max_t, 10.f);
}

TEST(Ray2f, GivenNormalizedRay_WhenCheckingIfRayIsNormalized_ThenNormalizedIsTrue) {
    Point2f origin{0.f, 0.f};
    Vector2f direction{1.f, 0.f};

    Ray2f r{origin, direction, 1.f, 10.f};

    EXPECT_TRUE(r.is_direction_vector_normalized());
}

TEST(Ray2f, GivenNonNormalizedRay_WhenCheckingIfRayIsNormalized_ThenNormalizedIsFalse) {
    Point2f origin{0.f, 0.f};
    Vector2f direction{2.f, 0.f};

    Ray2f r{origin, direction, 1.f, 10.f};

    EXPECT_FALSE(r.is_direction_vector_normalized());
}

TEST(Ray2f, GivenToShortNonNormalizedRay_WhenCheckingIfRayIsNormalized_ThenNormalizedIsFalse) {
    Point2f origin{0.f, 0.f};
    Vector2f direction{.1f, 0.f};
    Ray2f r{origin, direction, 1.f, 10.f};

    EXPECT_FALSE(r.is_direction_vector_normalized());
}

TEST(Ray2f, GivenANormalizedRay_WhenDeterminPointAtMinAndMaxT_ThenExpectStartAndEndPoint) {
    Point2f origin{0.f, 0.f};
    Vector2f direction{1.f, 0.f};

    Ray2f r(origin, direction, 1.f, 10.f);

    Point2f start = r(r.min_t);
    Point2f end = r(r.max_t);

    EXPECT_THAT(start.x(), 1.f);
    EXPECT_THAT(start.y(), 0.f);
    EXPECT_THAT(end.x(), 10.f);
    EXPECT_THAT(end.y(), 0.f);
}

TEST(Ray2f, GivenARay_WhenPrintedToStdOutput_ExpectRayAsStringRepresentation) {
    Point2f origin{0.f, 0.f};
    Vector2f direction{1.f, 0.f};

    Ray2f r{origin, direction, 1.f, 10.f};

    std::ostringstream ss;
    ss << r;
    std::string output = ss.str();

    EXPECT_THAT(output, testing::HasSubstr("Ray2f["));
    EXPECT_THAT(output, testing::HasSubstr("origin ="));
    EXPECT_THAT(output, testing::HasSubstr("direction ="));
    EXPECT_THAT(output, testing::HasSubstr("min_t = 1"));
    EXPECT_THAT(output, testing::HasSubstr("max_t = 10"));
    EXPECT_THAT(output, testing::HasSubstr("]"));
}

TEST(Ray2d, GivenARay2d_WhenPrintedToStdOutput_ExpectRayTypeRay2d) {
    Point2d origin{.0, 0.0};
    Vector2d direction{1.0, 0.0};
    Ray2d r{origin, direction, 1.0, 10.0};

    std::ostringstream ss;
    ss << r;
    std::string output = ss.str();

    EXPECT_THAT(output, testing::HasSubstr("Ray2d["));
    EXPECT_THAT(output, testing::HasSubstr("origin ="));
    EXPECT_THAT(output, testing::HasSubstr("direction ="));
    EXPECT_THAT(output, testing::HasSubstr("min_t = 1"));
    EXPECT_THAT(output, testing::HasSubstr("max_t = 10"));
    EXPECT_THAT(output, testing::HasSubstr("]"));
}

TEST(Ray3f, GivenARay3f_WhenPrintedToStdOutput_ExpectRayTypeRay3f) {
    Point3f origin{0.f, 0.f, 0.f};
    Vector3f direction{1.f, 0.f, 0.f};
    Ray3f r{origin, direction, 1.f, 10.f};

    std::ostringstream ss;
    ss << r;
    std::string output = ss.str();

    EXPECT_THAT(output, testing::HasSubstr("Ray3f["));
    EXPECT_THAT(output, testing::HasSubstr("origin ="));
    EXPECT_THAT(output, testing::HasSubstr("direction ="));
    EXPECT_THAT(output, testing::HasSubstr("min_t = 1"));
    EXPECT_THAT(output, testing::HasSubstr("max_t = 10"));
    EXPECT_THAT(output, testing::HasSubstr("]"));
}

TEST(internal_convertTypToString, GivenIntegralType_WhenConverTypeToString_ThenShortStringRepersentationExpected) {
    EXPECT_THAT(internal::convert_type_to_string<int>::value, 'i');
    EXPECT_THAT(internal::convert_type_to_string<float>::value, 'f');
    EXPECT_THAT(internal::convert_type_to_string<double>::value, 'd');
}

TEST(internal_dimension_as_string, GivenRayDimension_WhenConvertingToStringRepresentation_ThenExpectValidNumber) {
    EXPECT_THAT(internal::dimension_as_string<2u>::value, '2');
    EXPECT_THAT(internal::dimension_as_string<3u>::value, '3');
}
