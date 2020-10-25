/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Math_bfe8f1ea_9029_4b4e_b02a_7da908b1cdeb_h
#define Flatland_Math_bfe8f1ea_9029_4b4e_b02a_7da908b1cdeb_h

#include "flatland/core/namespace.h"

#include <cassert>
#include <cmath>

#if !defined(M_PI)
#define M_PI 3.14159265358979323846264338327950288
#endif

FLATLAND_BEGIN_NAMESPACE

template<class T> inline constexpr T pi_v = M_PI;

inline constexpr float pif = pi_v<float>;
inline constexpr double pi_over_2f = pi_v<float> / 2.0f;
inline constexpr double pi_over_4f = pi_v<float> / 4.0f;

inline constexpr double pi = pi_v<double>;
inline constexpr double pi_over_2 = pi_v<double> / 2.0;
inline constexpr double pi_over_4 = pi_v<double> / 4.0;

template <typename ScalarType>
ScalarType degreeToRadian(const ScalarType value) {
    static_assert(!std::numeric_limits<ScalarType>::is_exact, "degreeToRadian: floating-point type expected");
    return value * ScalarType{0.01745329251994329577};
}

template <typename ScalarType>
    ScalarType radianToDegree(const ScalarType value) {
    return value * ScalarType{57.295779513082320876798154814105};
}

template <typename ScalarType>
bool areEqual(const ScalarType a, const ScalarType b, const ScalarType epsilon) {
    return std::fabs(a - b) < epsilon;
}

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Math_bfe8f1ea_9029_4b4e_b02a_7da908b1cdeb_h
