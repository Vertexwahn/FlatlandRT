/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Intersection_391cb7cb_bd5e_4cf0_a115_66f7cc86f730_h
#define Flatland_Intersection_391cb7cb_bd5e_4cf0_a115_66f7cc86f730_h

#include "flatland/core/namespace.h"
#include "flatland/math/normal.h"
#include "flatland/math/point.h"
#include "flatland/math/vector.h"

FLATLAND_BEGIN_NAMESPACE

// https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm
template <typename ScalarType>
bool intersectRayCircle(
        const Point2<ScalarType>& rayOrigin,
        const Vector2<ScalarType>& rayDirection,
        const Point2<ScalarType>& circleCenter,
        ScalarType circleRadius,
        Point2<ScalarType>& intersectionPoint,
        ScalarType& t,
        Normal2<ScalarType>& normal) {
    Vector2<ScalarType> tmp = rayOrigin - circleCenter;
    ScalarType a = rayDirection.dot(rayDirection);
    ScalarType b = 2 * tmp.dot(rayDirection);
    ScalarType c = tmp.dot(tmp) - circleRadius * circleRadius;
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

    intersectionPoint = rayOrigin + t * rayDirection;
    ScalarType invRadius = ScalarType{1.0} / circleRadius;
    normal = (intersectionPoint - circleCenter) * invRadius;

    return true;
}

// https://rootllama.wordpress.com/2014/06/20/ray-line-segment-intersection-test-in-2d/
template <typename ScalarType>
bool intersectRayLineSegment(const Point2<ScalarType>& rayOrigin,
                             const Vector2<ScalarType>& rayDirection,
                             const Point2<ScalarType>& lineSegmentStart,
                             const Point2<ScalarType>& lineSegmentEnd,
                             Point2<ScalarType>& intersectionPoint,
                             ScalarType& t, // Parametric value at intersection point
                             Normal2<ScalarType>& n)    // Normal at intersection point
{
	auto v1 = rayOrigin - lineSegmentStart;
	auto v2 = lineSegmentEnd - lineSegmentStart;
	Vector2<ScalarType> v3{-rayDirection.y(), rayDirection.x()};
	auto t1 = crossProduct2(v2, v1) / v2.dot(v3);
	auto t2 = v1.dot(v3) / v2.dot(v3);

	if (t1 >= 0 && t2 >= 0 && t2 <= 1) {
		t = t1;
		intersectionPoint = rayOrigin + rayDirection * t1;
		n = Normal2<ScalarType>{v2.y(), -v2.x()};
		n.normalize();
		return true;
	}
	return false;
}

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Intersection_391cb7cb_bd5e_4cf0_a115_66f7cc86f730_h