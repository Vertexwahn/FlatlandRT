/*
 *  SPDX-FileCopyrightText: Copyright 2022-2025 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier:  Apache-2.0
 *
 *  This source file contains source code copied from pbrt-v4 (https://github.com/mmp/pbrt-v4)
 *  A copy of the original license can be found [here](/third_party/image_synthesis/pvrt-v4/LICENSE.txt).
 *  SPDX-FileCopyrightText: 1998-2020 Matt Pharr, Greg Humphreys, and Wenzel Jakob.
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  SPDX-FileCopyrightText: Copyright (c) 2023, LuisaGroup
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once
#ifndef De_Vertexwahn_Math_bfe8f1ea_9029_4b4e_b02a_7da908b1cdeb_h
#define De_Vertexwahn_Math_bfe8f1ea_9029_4b4e_b02a_7da908b1cdeb_h

#include "core/namespace.hpp"

#include "boost/predef.h"

#include <algorithm>
#include <bit>
#include <cassert>
#include <cmath>
#include <numbers>

DE_VERTEXWAHN_BEGIN_NAMESPACE

#ifndef EMSCRIPTEN
using std::lerp;
#endif
using std::clamp;

// the following function was copied from pbrt-v4 and slightly modified
template <typename T, typename U, typename V>
inline constexpr T PbrtClamp(T val, U low, V high) {
    if (val < low)
        return T(low);
    else if (val > high)
        return T(high);
    else
        return val;
}

// the following function was copied from pbrt-v4 and slightly modified
template <typename Predicate>
inline size_t FindInterval(size_t sz, const Predicate &pred) {
    using ssize_t = std::make_signed_t<size_t>;
    ssize_t size = (ssize_t)sz - 2, first = 1;
    while (size > 0) {
        // Evaluate predicate at midpoint and update _first_ and _size_
        size_t half = (size_t)size >> 1, middle = first + half;
        bool predResult = pred(middle);
        first = predResult ? middle + 1 : first;
        size = predResult ? size - (half + 1) : half;
    }
    return (size_t)PbrtClamp((ssize_t)first - 1, 0, sz - 2);
}

// the following function was copied from pbrt-v4 and slightly modified
template <typename ScalarType>
inline constexpr ScalarType sqr(ScalarType value) {
    return value * value;
}

// TODO: Remove this in favour of std::clamp
template <typename T, typename U, typename V>
inline constexpr T clamp(T val, U low, V high) {
    if (val < low)
        return T(low);
    else if (val > high)
        return T(high);
    else
        return val;
}

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

// The following function next_pow2 has been copied from https://github.com/LuisaGroup/LuisaCompute
// and is licensed under the following license:
/*
    BSD 3-Clause License

    Copyright (c) 2023, LuisaGroup
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**
 * @brief Find next 2^n of v
 *
 * @tparam uint32 or uint64
 * @param v input number
 * @return same as v
 */
template<typename T, std::enable_if_t<std::is_unsigned_v<T> && (sizeof(T) == 4u || sizeof(T) == 8u), int> = 0>
[[nodiscard]] constexpr auto next_pow2(T v) noexcept {
#ifdef __cpp_lib_int_pow2
    return std::bit_ceil(v);
#else
    v--;
    v |= v >> 1u;
    v |= v >> 2u;
    v |= v >> 4u;
    v |= v >> 8u;
    v |= v >> 16u;
    if constexpr (sizeof(T) == 8u) { v |= v >> 32u; }
    return v + 1u;
#endif
}

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Math_bfe8f1ea_9029_4b4e_b02a_7da908b1cdeb_h
