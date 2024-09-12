/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "math/intersection.hpp"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(Intersection, IntersectRayCircle_NoHit) {
    Point2f ray_origin(0.f, 100.f);
    Vector2f ray_direction(-1.f, 0.f); // shoot away - expected is no hit
    Point2f circle_center(100.f, 100.f);
    float radius = 50.f;

    Point2f intersection_point;
    Normal2f normal;
    float t;

    bool bHit = intersect_ray_n_sphere(
            ray_origin,
            ray_direction,
            circle_center,
            radius,
            intersection_point,
            t,
            normal);

    EXPECT_FALSE(bHit);
}

TEST(Intersection, IntersectRayCircle) {
    Point2f ray_origin(0.f, 100.f);
    Vector2f ray_direction(1.f, 0.f); // shoot away - expected is no hit
    Point2f circle_center(100.f, 100.f);
    float radius = 50.f;

    Point2f intersection_point;
    Normal2f normal;
    float t = 0.f;

    bool bHit = intersect_ray_n_sphere(
            ray_origin,
            ray_direction,
            circle_center,
            radius,
            intersection_point,
            t,
            normal);

    EXPECT_THAT(t, testing::FloatEq(50.f));

    EXPECT_THAT(intersection_point.x(), testing::FloatEq(50.f));
    EXPECT_THAT(intersection_point.y(), testing::FloatEq(100.f));

    EXPECT_TRUE(bHit);
}

TEST(Intersection, RayLineSegmentIntersection) {
    const Point2f ray_origin(0.f, 0.f);
    const Vector2f ray_direction(1.f, 0.f);
    const Point2f line_segment_start(100.f, -100.f);
    const Point2f line_segment_end(100.f, 100.f);
    float t;
    Normal2f n;

    Point2f intersection_point;

    bool result = intersect_ray_line_segment(ray_origin, ray_direction, line_segment_start, line_segment_end,
                                             intersection_point, t, n);

    EXPECT_TRUE(result);
    EXPECT_THAT(intersection_point.x(), testing::FloatEq(100.f));
    EXPECT_THAT(intersection_point.y(), testing::FloatEq(0.f));

    EXPECT_THAT(n.x(), testing::FloatEq(1.f));
    EXPECT_THAT(n.y(), testing::FloatEq(0.f));
}

TEST(Intersection, RayLineSegmentIntersection2) {
    const Point2f ray_origin(0.f, 0.f);
    const Vector2f ray_direction(1.f, -1.f);
    const Point2f line_segment_start(100.f, -100.f);
    const Point2f line_segment_end(300.f, 100.f);
    float t;
    Normal2f n;

    Point2f intersection_point;

    bool result = intersect_ray_line_segment(ray_origin, ray_direction, line_segment_start, line_segment_end,
                                             intersection_point, t, n);

    EXPECT_TRUE(result);

    EXPECT_THAT(n.x(), testing::FloatEq(std::sqrt(.5f)));
    EXPECT_THAT(n.y(), testing::FloatEq(-std::sqrt(.5f)));
}
