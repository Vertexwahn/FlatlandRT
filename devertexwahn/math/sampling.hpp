/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Math_warping_50c70bb0_0a28_43db_a7c7_03aaec4146cd_h
#define De_Vertexwahn_Math_warping_50c70bb0_0a28_43db_a7c7_03aaec4146cd_h

#include "math/constants.hpp"
#include "math/point.hpp"
#include "math/util.hpp"
#include "math/vector.hpp"

#include "core/namespace.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template <typename ScalarType>
Point2<ScalarType> warp_uniform_square_to_2x(const Point2<ScalarType> &sample) {
    auto p_1_inverse = [](const ScalarType x) {
        if(x < ScalarType{0}) {
            return ScalarType{0};
        }
        if(x > ScalarType{1}) {
            return ScalarType{1};
        }
        return std::sqrt(x);
    };

    return Point2<ScalarType>(p_1_inverse(sample.x()), p_1_inverse(sample.y()));
}

template <typename ScalarType>
ScalarType warp_uniform_square_to_2x_pdf(const Point2<ScalarType> &sample) {
    auto p_1 = [](const ScalarType x) {
        if(x < ScalarType{0}) {
            return ScalarType{0};
        }
        if(x > ScalarType{1}) {
            return ScalarType{1};
        }
        return ScalarType{2} * x;
    };
    return p_1(sample.x()) * p_1(sample.y());
}


// The function SampleUniformDiskConcentric has been copied from pbrt-v4
// and slightly modified
// pbrt is Copyright(c) 1998-2020 Matt Pharr, Wenzel Jakob, and Greg Humphreys.
// The pbrt source code is licensed under the Apache License, Version 2.0.
// The license can be found here: licenses/pbrt-v4/
template <typename ScalarType>
Point2<ScalarType> warp_uniform_square_to_concentric_disk(const Point2<ScalarType> &sample) {
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
ScalarType warp_uniform_square_to_concentric_disk_pdf(const Point2<ScalarType> &p) {
    if (p.x() * p.x() + p.y() * p.y() <= 1)
        return ScalarType{1} / pi_v<ScalarType>;
    else
        return ScalarType{0};
}

template <typename ScalarType>
Vector2<ScalarType> rotated_unit_vector(const ScalarType radian) {
    ScalarType x = cos(radian);
    ScalarType y = sin(radian);
    Vector2<ScalarType> v{x, y};
    v.normalize();
    return v;
}

template <typename ScalarType>
Vector2<ScalarType> uniform_sample_half_circle(const ScalarType value) {
    ScalarType angle = value * ScalarType{180.0};
    return rotated_unit_vector(degree_to_radian(angle));
}

template <typename ScalarType>
Vector2<ScalarType> sample_half_circle(const Point2<ScalarType> &sample) {
    Point2<ScalarType> d = warp_uniform_square_to_concentric_disk(sample);
    if(d.y() < ScalarType{0.0}) {
        d.y() = -d.y();
    }

    return Vector2<ScalarType>(d.x(), d.y());
}


DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Math_warping_50c70bb0_0a28_43db_a7c7_03aaec4146cd_h
