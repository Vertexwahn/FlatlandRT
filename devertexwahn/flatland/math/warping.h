/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Math_warping_50c70bb0_0a28_43db_a7c7_03aaec4146cd_h
#define Flatland_Math_warping_50c70bb0_0a28_43db_a7c7_03aaec4146cd_h

#include "flatland/math/point.h"
#include "flatland/math/util.h"
#include "flatland/math/vector.h"
#include "flatland/core/namespace.h"

FLATLAND_BEGIN_NAMESPACE

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

template <typename ScalarType>
Point2<ScalarType> warp_uniform_square_to_tent(const Point2<ScalarType> &sample) {
    auto p_1_inverse = [](const ScalarType t) {
        assert(t >= 0);
        assert(t <= 1);

        if (t >= ScalarType{0} && t < ScalarType{0.5}) {
            return -ScalarType{1} + std::sqrt(ScalarType{2} * t);
        } else if (t >= 0.5 && t < ScalarType{1}) {
            return ScalarType{1} - std::sqrt(ScalarType{2} * (ScalarType{1} - t));
        } else {
            throw std::runtime_error("Invalid parameter provided");
        }
    };

    return Point2<ScalarType>(p_1_inverse(sample.x()), p_1_inverse(sample.y()));
}

template <typename ScalarType>
ScalarType warp_uniform_square_to_tent_pdf(const Point2<ScalarType> &sample) {
    auto p_1 = [](const ScalarType t) {
        assert(t >= -ScalarType{1});
        assert(t <= ScalarType{1});
        return ScalarType{1} - std::abs(t);
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
        return ScalarType{1} / ScalarType{M_PI};
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


template <typename ScalarType>
Vector3<ScalarType> square_to_cosine_hemisphere(const Point2<ScalarType> &sample) {
    // shamelessly taken from pbrt-v3
    Point2<ScalarType> d = warp_uniform_square_to_concentric_disk(sample);
    ScalarType z = std::sqrt(std::max(ScalarType{0}, ScalarType{1} - d.x()*d.x() - d.y()*d.y()));
    return Vector3<ScalarType>{d.x(), d.y(), z};
}

template <typename ScalarType>
ScalarType square_to_cosine_hemisphere_pdf(const Vector3<ScalarType> &v) {
    if (v.z() < ScalarType{0.}) {
        return ScalarType{0.};
    }
    else {
        // Transform v to polar coordinates
        ScalarType r = v.norm();
        ScalarType theta = acos(v.z() / r);
        ScalarType cosTheta = cos(theta);
        return cosTheta * (ScalarType{1.} / ScalarType{M_PI});
    }
}

template <typename ScalarType>
Vector3<ScalarType> square_to_uniform_hemisphere(const Point2<ScalarType> &sample) {
    // samples are generated to the hemisphere centered at (0,0,1)

    // taken from PBRT
    ScalarType z = sample.x();
    ScalarType r = std::sqrt(std::max(ScalarType{0}, ScalarType{1} - z * z));
    ScalarType phi = ScalarType{2} * ScalarType{M_PI} * sample.y();
    return Vector3<ScalarType>(r * std::cos(phi), r * std::sin(phi), z);
}

template <typename ScalarType>
ScalarType square_to_uniform_hemisphere_pdf(const Vector3<ScalarType> &sample) {
    if (sample.z() >= 0.f) {
        return ScalarType{1}/(ScalarType{2}*M_PI);
    }
    else {
        return ScalarType{0};
    }
}

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Math_warping_50c70bb0_0a28_43db_a7c7_03aaec4146cd_h