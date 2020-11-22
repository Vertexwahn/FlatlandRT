/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/scene/shape/disk.h"
#include "flatland/math/util.h"
#include "flatland/math/intersection.h"
#include "flatland/scene/shape/rectangle.h"
#include "flatland/core/reference_counted.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(Intersection, IntersectRayCircle_NoHit) {
	Point2f rayOrigin(0.0f, 100.0f);
	Vector2f rayDirection(-1.0f, 0.0f); // shoot away - expected is no hit
	Point2f circleCenter(100.0f, 100.0f);
	float radius = 50.0f;

	Point2f intersectionPoint;
	Normal2f normal;
	float t;

	bool bHit = intersectRayCircle(
	        rayOrigin,
	        rayDirection,
	        circleCenter,
	        radius,
	        intersectionPoint,
	        t,
	        normal);

	EXPECT_FALSE(bHit);
}

TEST(Intersection, IntersectRayCircle) {
	Point2f rayOrigin(0.0f, 100.0f);
	Vector2f rayDirection(1.0f, 0.0f); // shoot away - expected is no hit
	Point2f circleCenter(100.0f, 100.0f);
	float radius = 50.0f;

	Point2f intersectionPoint;
	Normal2f normal;
	float t;

	bool bHit = intersectRayCircle(
	        rayOrigin,
	        rayDirection,
	        circleCenter,
	        radius,
	        intersectionPoint,
	        t,
	        normal);

	EXPECT_THAT(t, testing::FloatEq(50.0f));

	EXPECT_THAT(intersectionPoint.x(), testing::FloatEq(50.0f));
	EXPECT_THAT(intersectionPoint.y(), testing::FloatEq(100.0f));

	EXPECT_TRUE(bHit);
}

TEST(Intersection, RayLineSegmentIntersection) {
	const Point2f rayOrigin(0.0f, 0.0f);
	const Vector2f rayDirection(1.0f, 0.0f);
	const Point2f lineSegmentStart(100.0f, -100.0f);
	const Point2f lineSegmentEnd(100.0f, 100.0f);
    float t;
    Normal2f n;

	Point2f intersectionPoint;

	bool result = intersectRayLineSegment(rayOrigin, rayDirection, lineSegmentStart, lineSegmentEnd, intersectionPoint, t, n);

	EXPECT_TRUE(result);
    EXPECT_THAT(intersectionPoint.x(), testing::FloatEq(100.0f));
    EXPECT_THAT(intersectionPoint.y(), testing::FloatEq(0.0f));

    EXPECT_THAT(n.x(), testing::FloatEq(1.0f));
    EXPECT_THAT(n.y(), testing::FloatEq(0.0f));
}

TEST(Intersection, RayLineSegmentIntersection2) {
    const Point2f rayOrigin(0.0f, 0.0f);
    const Vector2f rayDirection(1.0f, -1.0f);
    const Point2f lineSegmentStart(100.0f, -100.0f);
    const Point2f lineSegmentEnd(300.0f, 100.0f);
    float t;
    Normal2f n;

    Point2f intersectionPoint;

    bool result = intersectRayLineSegment(rayOrigin, rayDirection, lineSegmentStart, lineSegmentEnd, intersectionPoint, t, n);

    EXPECT_TRUE(result);

    EXPECT_THAT(n.x(), testing::FloatEq(sqrt(0.5f)));
    EXPECT_THAT(n.y(), testing::FloatEq(-sqrt(0.5f)));
}

TEST(Intersection2f, Ray2CircleIntersection) {
    float length = 100.0f;

    Ray2f r(Point2f(0.0f, 100.0f), Vector2f(1.0f, 0.0f), 0.0f, length);

    PropertySet ps;
    ps.addProperty("radius", 50.0f);
    ps.addProperty("transform", translate(Vector2f{100.0f, 100.0f}));

    Disk2f c{ps};

    MediumEvent2f its;
    bool hit = c.intersect(r, its);

    ASSERT_TRUE(hit);
    EXPECT_THAT(its.t, testing::FloatEq(50.0f));
}