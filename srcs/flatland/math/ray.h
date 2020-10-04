/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Ray_ceb9b0b4_4236_4bfe_b918_11a02c72ad7c_h
#define Flatland_Ray_ceb9b0b4_4236_4bfe_b918_11a02c72ad7c_h

#include "flatland/core/namespace.h"
#include "flatland/math/point.h"
#include "flatland/math/vector.h"

#include <iostream>
#include <type_traits>
#include <string>

FLATLAND_BEGIN_NAMESPACE

template <unsigned int Dimension, typename ScalarType>
struct RayType {
    using Point = PointType<Dimension, ScalarType>;
    using Vector = VectorType<Dimension, ScalarType>;
    using Scalar = ScalarType;

    RayType(const Point &origin, const Vector &direction, const Scalar min_t, const Scalar max_t)
            : origin(origin), direction(direction), min_t(min_t), max_t(max_t) {
    }

    Point operator()(const float t) const {
        assert(isDirectionVectorNormalized());
        return origin + t * direction;
    }

    bool isDirectionVectorNormalized() const {
        const Scalar epsilon{0.001};
        return std::abs(direction.norm() - Scalar{1.0}) < epsilon;
    }

    Point origin;
    Vector direction;
    Scalar min_t;
    Scalar max_t;

    constexpr static unsigned int dimension = Dimension;
};

template <typename ScalarType>
using Ray2 = RayType<2, ScalarType>;

template <typename ScalarType>
using Ray3 = RayType<3, ScalarType>;

using Ray2f = Ray2<float>;
using Ray2d = Ray2<double>;
using Ray3f = Ray3<float>;
using Ray3d = Ray3<double>;

namespace internal {
    template<typename ScalarType>
    struct convertTypeToString;

    template<>
    struct convertTypeToString<int> {
        constexpr static char value = 'i';
    };

    template<>
    struct convertTypeToString<float> {
        constexpr static char value = 'f';
    };

    template<>
    struct convertTypeToString<double> {
        constexpr static char value = 'd';
    };

    template<unsigned int Dimension>
    struct dimensionAsString;

    template<>
    struct dimensionAsString<2u> {
        constexpr static char value = '2';
    };

    template<>
    struct dimensionAsString<3u> {
        constexpr static char value = '3';
    };
}

template <typename  ScalarType, unsigned int Dimension>
std::ostream &operator<<(std::ostream &os, const RayType<Dimension, ScalarType> &r) {
    os << "Ray"
       << internal::dimensionAsString<Dimension>::value
       << internal::convertTypeToString<ScalarType>::value
       << "[" << std::endl
       << "  origin = " << r.origin << "," << std::endl
       << "  direction = " << r.direction << "," << std::endl
       << "  min_t = " << r.min_t << "," << std::endl
       << "  max_t = " << r.max_t << "" << std::endl
       << "]" << std::endl;

    return os;
}

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Ray_ceb9b0b4_4236_4bfe_b918_11a02c72ad7c_h
