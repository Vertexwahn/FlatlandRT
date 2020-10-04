/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/math/ray.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(Ray2f, GivenDirectionOriginMaxMinT_WhenRayIsInitialized_ThenInitzalizedRayValues) {
    Point2f origin(0.0f, 0.0f);
	Vector2f direction(1.0f, 0.0f);

	Ray2f r(origin, direction, 1.0f, 10.0f);

    EXPECT_THAT(r.origin.x(), 0.0f);
    EXPECT_THAT(r.origin.y(), 0.0f);
    EXPECT_THAT(r.direction.x(), 1.0f);
    EXPECT_THAT(r.direction.y(), 0.0f);
    EXPECT_THAT(r.min_t, 1.0f);
	EXPECT_THAT(r.max_t, 10.0f);
}

TEST(Ray2f, GivenNormalizedRay_WhenCheckingIfRayIsNormalized_ThenNormalizedIsTrue) {
    Point2f origin(0.0f, 0.0f);
    Vector2f direction(1.0f, 0.0f);

    Ray2f r(origin, direction, 1.0f, 10.0f);

    EXPECT_TRUE(r.isDirectionVectorNormalized());
}

TEST(Ray2f, GivenNonNormalizedRay_WhenCheckingIfRayIsNormalized_ThenNormalizedIsFalse) {
    Point2f origin(0.0f, 0.0f);
    Vector2f direction(2.0f, 0.0f);

    Ray2f r(origin, direction, 1.0f, 10.0f);

    EXPECT_FALSE(r.isDirectionVectorNormalized());
}

TEST(Ray2f, GivenToShortNonNormalizedRay_WhenCheckingIfRayIsNormalized_ThenNormalizedIsFalse) {
    Point2f origin(0.0f, 0.0f);
    Vector2f direction(0.1f, 0.0f);
    Ray2f r(origin, direction, 1.0f, 10.0f);

    EXPECT_FALSE(r.isDirectionVectorNormalized());
}

TEST(Ray2f, GivenANormalizedRay_WhenDeterminPointAtMinAndMaxT_ThenExpectStartAndEndPoint) {
    Point2f origin(0.0f, 0.0f);
    Vector2f direction(1.0f, 0.0f);

    Ray2f r(origin, direction, 1.0f, 10.0f);

    Point2f start = r(r.min_t);
    Point2f end = r(r.max_t);

    EXPECT_THAT(start.x(), 1.0f);
    EXPECT_THAT(start.y(), 0.0f);
    EXPECT_THAT(end.x(), 10.0f);
    EXPECT_THAT(end.y(), 0.0f);
}

TEST(Ray2f, GivenARay_WhenPrintedToStdOutput_ExpectRayAsStringRepresentation) {
    Point2f origin(0.0f, 0.0f);
    Vector2f direction(1.0f, 0.0f);

    Ray2f r(origin, direction, 1.0f, 10.0f);

    std::ostringstream ss;
    ss << r;
    std::string output = ss.str();

    EXPECT_THAT(output, ::testing::HasSubstr("Ray2f["));
    EXPECT_THAT(output, ::testing::HasSubstr("origin ="));
    EXPECT_THAT(output, ::testing::HasSubstr("direction ="));
    EXPECT_THAT(output, ::testing::HasSubstr("min_t = 1"));
    EXPECT_THAT(output, ::testing::HasSubstr("max_t = 10"));
    EXPECT_THAT(output, ::testing::HasSubstr("]"));
}

TEST(Ray2d, GivenARay2d_WhenPrintedToStdOutput_ExpectRayTypeRay2d) {
    Point2d origin(0.0, 0.0);
    Vector2d direction(1.0, 0.0);
    Ray2d r(origin, direction, 1.0, 10.0);

    std::ostringstream ss;
    ss << r;
    std::string output = ss.str();

    EXPECT_THAT(output, ::testing::HasSubstr("Ray2d["));
    EXPECT_THAT(output, ::testing::HasSubstr("origin ="));
    EXPECT_THAT(output, ::testing::HasSubstr("direction ="));
    EXPECT_THAT(output, ::testing::HasSubstr("min_t = 1"));
    EXPECT_THAT(output, ::testing::HasSubstr("max_t = 10"));
    EXPECT_THAT(output, ::testing::HasSubstr("]"));
}

TEST(Ray3f, GivenARay3f_WhenPrintedToStdOutput_ExpectRayTypeRay3f) {
    Point3f origin(0.0f, 0.0f, 0.0f);
    Vector3f direction(1.0f, 0.0f, 0.0f);
    Ray3f r(origin, direction, 1.0f, 10.0f);

    std::ostringstream ss;
    ss << r;
    std::string output = ss.str();

    EXPECT_THAT(output, ::testing::HasSubstr("Ray3f["));
    EXPECT_THAT(output, ::testing::HasSubstr("origin ="));
    EXPECT_THAT(output, ::testing::HasSubstr("direction ="));
    EXPECT_THAT(output, ::testing::HasSubstr("min_t = 1"));
    EXPECT_THAT(output, ::testing::HasSubstr("max_t = 10"));
    EXPECT_THAT(output, ::testing::HasSubstr("]"));
}

TEST(internal_convertTypToString, GivenIntegralType_WhenConverTypeToString_ThenShortStringRepersentationExpected) {
    EXPECT_THAT(internal::convertTypeToString<int>::value, 'i');
    EXPECT_THAT(internal::convertTypeToString<float>::value, 'f');
    EXPECT_THAT(internal::convertTypeToString<double>::value, 'd');
}

TEST(internal_dimensionAsString, GivenRayDimension_WhenConvertingToStringRepresentation_ThenExpectValidNumber) {
    EXPECT_THAT(internal::dimensionAsString<2u>::value, '2');
    EXPECT_THAT(internal::dimensionAsString<3u>::value, '3');
}