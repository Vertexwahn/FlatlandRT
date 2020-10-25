/*
    This file is part of Flatland, a simple 2D ray tracer
*/

#pragma once
#ifndef Flatland_warping_50c70bb0_0a28_43db_a7c7_03aaec4146cd_h
#define Flatland_warping_50c70bb0_0a28_43db_a7c7_03aaec4146cd_h

#include "flatland/core/namespace.h"
#include "flatland/math/point.h"
#include "flatland/math/util.h"
#include "flatland/math/vector.h"

FLATLAND_BEGIN_NAMESPACE

// The function SampleUniformDiskConcentric has been copied from pbrt-v4
// and slightly modified
// pbrt is Copyright(c) 1998-2020 Matt Pharr, Wenzel Jakob, and Greg Humphreys.
// The pbrt source code is licensed under the Apache License, Version 2.0.
// The license can be found here: licenses/pbrt-v4/
template <typename ScalarType>
Point2<ScalarType> warpUniformSquareToConcentricDisk(const Point2<ScalarType> &sample) {
    // Map _u_ to $[-1,1]^2$ and handle degeneracy at the origin
    Point2<ScalarType> uOffset = 2.0 * sample - Vector2<ScalarType>(1, 1);
    if (uOffset.x() == 0 && uOffset.y() == 0)
        return {0, 0};

    // Apply concentric mapping to point
    ScalarType theta, r;
    if (std::abs(uOffset.x()) > std::abs(uOffset.y())) {
        r = uOffset.x();
        theta = pi_over_4 * (uOffset.y() / uOffset.x());
    } else {
        r = uOffset.y();
        theta = pi_over_2 - pi_over_4 * (uOffset.x() / uOffset.y());
    }
    return r * Point2<ScalarType>(std::cos(theta), std::sin(theta));
}

template <typename ScalarType>
Vector2<ScalarType> rotatedUnitVector(const ScalarType radian) {
    ScalarType x = cos(radian);
    ScalarType y = sin(radian);
    Vector2<ScalarType> v{x, y};
    v.normalize();
    return v;
}

template <typename ScalarType>
Vector2<ScalarType> uniformSampleHalfCircle(const ScalarType value) {
    ScalarType angle = value * ScalarType{180.0};
    return rotatedUnitVector(degreeToRadian(angle));
}

template <typename ScalarType>
Vector2<ScalarType> sampleHalfCircle(const Point2<ScalarType> &sample) {
    Point2<ScalarType> d = warpUniformSquareToConcentricDisk(sample);
    if(d.y() < ScalarType{0.0}) {
        d.y() = -d.y();
    }

    return Vector2<ScalarType>(d.x(), d.y());
}

FLATLAND_END_NAMESPACE

#endif // end define Flatland_warping_50c70bb0_0a28_43db_a7c7_03aaec4146cd_h