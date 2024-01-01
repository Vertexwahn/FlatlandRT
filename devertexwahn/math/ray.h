/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Math_Ray_ceb9b0b4_4236_4bfe_b918_11a02c72ad7c_h
#define De_Vertexwahn_Math_Ray_ceb9b0b4_4236_4bfe_b918_11a02c72ad7c_h

#include "math/point.h"
#include "math/vector.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template <typename ScalarType, unsigned int Dimension>
struct RayType {
    using Scalar = ScalarType;
    using Point = PointType<ScalarType, Dimension>;
    using Vector = VectorType<ScalarType, Dimension>;

    RayType(const Point &origin, const Vector &direction, const Scalar min_t, const Scalar max_t)
            : origin(origin), direction(direction), min_t(min_t), max_t(max_t) {
    }

    Point operator()(const float t) const {
        assert(is_direction_vector_normalized());
        return origin + t * direction;
    }

    [[nodiscard]]
    bool is_direction_vector_normalized() const {
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
using Ray2 = RayType<ScalarType, 2>;

template <typename ScalarType>
using Ray3 = RayType<ScalarType, 3>;

using Ray2f = Ray2<float>;
using Ray2d = Ray2<double>;
using Ray3f = Ray3<float>;
using Ray3d = Ray3<double>;

namespace internal {
    template<typename ScalarType>
    struct convert_type_to_string;

    template<>
    struct convert_type_to_string<int> {
        constexpr static char value = 'i';
    };

    template<>
    struct convert_type_to_string<float> {
        constexpr static char value = 'f';
    };

    template<>
    struct convert_type_to_string<double> {
        constexpr static char value = 'd';
    };

    template<unsigned int Dimension>
    struct dimension_as_string;

    template<>
    struct dimension_as_string<2u> {
        constexpr static char value = '2';
    };

    template<>
    struct dimension_as_string<3u> {
        constexpr static char value = '3';
    };
}

template <typename  ScalarType, unsigned int Dimension>
std::ostream &operator<<(std::ostream &os, const RayType<ScalarType, Dimension> &r) {
    os << "Ray"
       << internal::dimension_as_string<Dimension>::value
       << internal::convert_type_to_string<ScalarType>::value
       << "[" << std::endl
       << "  origin = " << r.origin << "," << std::endl
       << "  direction = " << r.direction << "," << std::endl
       << "  min_t = " << r.min_t << "," << std::endl
       << "  max_t = " << r.max_t << "" << std::endl
       << "]" << std::endl;

    return os;
}

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Math_Ray_ceb9b0b4_4236_4bfe_b918_11a02c72ad7c_h
