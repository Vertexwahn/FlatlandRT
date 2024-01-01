/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Math_Intersection_391cb7cb_bd5e_4cf0_a115_66f7cc86f730_h
#define De_Vertexwahn_Math_Intersection_391cb7cb_bd5e_4cf0_a115_66f7cc86f730_h

#include "math/normal.h"
#include "math/point.h"
#include "math/vector.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

// https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm
template <typename ScalarType, unsigned int Dimension>
bool intersect_ray_n_sphere(
        const PointType<ScalarType, Dimension>& ray_origin,
        const VectorType<ScalarType, Dimension>& ray_direction,
        const PointType<ScalarType, Dimension>& circle_center,
        ScalarType circle_radius,
        PointType<ScalarType, Dimension>& intersection_point,
        ScalarType& t,
        NormalType<ScalarType, Dimension>& normal /* world space normal */) {
    VectorType<ScalarType, Dimension> tmp = ray_origin - circle_center;
    ScalarType a = ray_direction.dot(ray_direction);
    ScalarType b = 2 * tmp.dot(ray_direction);
    ScalarType c = tmp.dot(tmp) - circle_radius * circle_radius;
    ScalarType discriminant = b*b-4*a*c;

    if( discriminant < 0 ) {
        return  false;
    }

    ScalarType t1 = (-b + sqrt(discriminant)) / (2*a);
    ScalarType t2 = (-b - sqrt(discriminant)) / (2*a);

    if (t1 < 0 && t2 < 0) {
        return false;
    }

    if (t1 >= 0 && t2 >= 0) {
        // find smaller t
        if (t1 < t2) {
            t = t1;
        }
        else {
            t = t2;
        }
    }

    if (t1 < 0 && t2 >= 0) {
        t = t2;
    }

    if (t1 >= 0 && t2 < 0) {
        t = t1;
    }

    intersection_point = ray_origin + t * ray_direction;
    ScalarType inverse_radius = ScalarType{1.0} / circle_radius;
    normal = (intersection_point - circle_center) * inverse_radius;

    return true;
}

// https://rootllama.wordpress.com/2014/06/20/ray-line-segment-intersection-test-in-2d/
template <typename ScalarType>
bool intersect_ray_line_segment(const Point2<ScalarType>& ray_origin,
                                const Vector2<ScalarType>& ray_direction,
                                const Point2<ScalarType>& line_segment_start,
                                const Point2<ScalarType>& line_segment_end,
                                Point2<ScalarType>& intersection_point,
                                ScalarType& t, // Parametric value at intersection point
                             Normal2<ScalarType>& n)    // Normal at intersection point
{
	auto v1 = ray_origin - line_segment_start;
	auto v2 = line_segment_end - line_segment_start;
	Vector2<ScalarType> v3{-ray_direction.y(), ray_direction.x()};
	auto t1 = cross_product_2(v2, v1) / v2.dot(v3);
	auto t2 = v1.dot(v3) / v2.dot(v3);

	if (t1 >= 0 && t2 >= 0 && t2 <= 1) {
		t = t1;
        intersection_point = ray_origin + ray_direction * t1;
		n = Normal2<ScalarType>{v2.y(), -v2.x()};
		n.normalize();
		return true;
	}
	return false;
}

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Math_Intersection_391cb7cb_bd5e_4cf0_a115_66f7cc86f730_h
