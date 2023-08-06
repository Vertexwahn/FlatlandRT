/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Math_bfe8f1ea_9029_4b4e_b02a_7da908b1cdeb_h
#define De_Vertexwahn_Math_bfe8f1ea_9029_4b4e_b02a_7da908b1cdeb_h

#include "core/namespace.h"

#include "boost/predef.h"

#include <algorithm>
#include <cassert>
#include <cmath>

#if BOOST_COMP_GNUC < BOOST_VERSION_NUMBER(10,0,0) // GCC9 does not support #include <numbers>
// do nothing...
#else
#include <numbers>
#endif

DE_VERTEXWAHN_BEGIN_NAMESPACE

#if BOOST_COMP_GNUC < BOOST_VERSION_NUMBER(10,0,0) // GCC9 does not support std::numbers::pi with
template<typename T>
constexpr T pi_v = static_cast<T>(3.141592653589793238462643383279502884L);
template<typename T>
constexpr T inv_pi_v = static_cast<T>(0.318309886183790671537767526745028724L);
#else
template<typename ScalarType>
constexpr ScalarType pi_v = std::numbers::pi_v<ScalarType>;
template<typename ScalarType>
constexpr ScalarType inv_pi_v = std::numbers::inv_pi_v<ScalarType>;
#endif

inline constexpr float pif = pi_v<float>;
inline constexpr float pi_over_2f = pi_v<float> / 2.f;
inline constexpr float pi_over_4f = pi_v<float> / 4.f;
inline constexpr float inv_pif = inv_pi_v<float>;
inline constexpr float inv_2_pif = 1.f / (2.f * pi_v<float>);

inline constexpr double pi = pi_v<double>;
inline constexpr double pi_over_2 = pi_v<double> / 2.0;
inline constexpr double pi_over_4 = pi_v<double> / 4.0;
inline constexpr double inv_pid = inv_pi_v<double>;
inline constexpr double inv_2_pid = 1.0 / 2.0 * pi_v<double>;

template <typename ScalarType>
ScalarType degree_to_radian(const ScalarType value) {
    static_assert(!std::numeric_limits<ScalarType>::is_exact, "degree_to_radian: floating-point type expected");
    return value * ScalarType{0.01745329251994329577};
}

template <typename ScalarType>
ScalarType radian_to_degree(const ScalarType value) {
    return value * ScalarType{57.295779513082320876798154814105};
}

template <typename ScalarType>
bool are_equal(const ScalarType a, const ScalarType b, const ScalarType epsilon) {
    return std::fabs(a - b) < epsilon;
}

template<typename PointType>
PointType mid_point(PointType p1, PointType p2) {
    //assert(p1 <= p2);
    return p1 + (p2 - p1) * .5f;
}

// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2485.pdf
template<typename T>
const T &min(const T &a) {
    return a;
}

//template< LessThanComparable T, typename ... Args>
template< typename T, typename ... Args>
//requires SameType<T, Args>...
const T &
min(const T &a, const T &b, const Args &... args) {
    return std::min(b < a ? b : a, args ...);
}

template<typename T>
const T &max(const T &a) {
    return a;
}

//template< LessThanComparable T, typename ... Args>
template< typename T, typename ... Args>
//requires SameType<T, Args>...
const T &
max(const T &a, const T &b, const Args &... args) {
    return std::max(b > a ? b : a, args ...);
}

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Math_bfe8f1ea_9029_4b4e_b02a_7da908b1cdeb_h
