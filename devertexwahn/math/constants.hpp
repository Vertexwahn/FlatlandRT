/*
 *  SPDX-FileCopyrightText: Copyright 2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_constants_ca08c7db_253c_4a8a_87fd_13614a543dfd_h
#define De_Vertexwahn_Core_constants_ca08c7db_253c_4a8a_87fd_13614a543dfd_h

#include "boost/predef.h"

#include "core/namespace.hpp"

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

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Core_constants_ca08c7db_253c_4a8a_87fd_13614a543dfd_h
