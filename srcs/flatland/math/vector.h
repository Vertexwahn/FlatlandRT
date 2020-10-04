/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Vector_0d3445ef_7a2f_4c97_bf41_fd6b1e54710c_h
#define Flatland_Vector_0d3445ef_7a2f_4c97_bf41_fd6b1e54710c_h

#include "flatland/core/namespace.h"

#include <Eigen/Core>
#include <Eigen/Dense>

FLATLAND_BEGIN_NAMESPACE

template<unsigned int Dimension, typename ScalarType>
struct VectorType : public Eigen::Matrix<ScalarType, Dimension, 1> {
    using Base = Eigen::Matrix<ScalarType, Dimension, 1>;

    VectorType() : Base() {}
    VectorType(const ScalarType x, const ScalarType y) : Base(x,y) {}
    VectorType(const ScalarType x, const ScalarType y, const ScalarType z) : Base(x,y,z) {}
    VectorType(const ScalarType x, const ScalarType y, const ScalarType z, const ScalarType w) : Base(x,y,z,w) {}

    template <typename Derived>
    VectorType(const Eigen::MatrixBase<Derived>& src) : Base(src) {
    }

    using Base::operator=;
};

template <typename ScalarType>
using Vector2 = VectorType<2, ScalarType>;
template <typename ScalarType>
using Vector3 = VectorType<3, ScalarType>;
template <typename ScalarType>
using Vector4 = VectorType<4, ScalarType>;

using Vector2i = Vector2<int>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;
using Vector3i = Vector3<int>;
using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;
using Vector4i = Vector4<int>;
using Vector4f = Vector4<float>;
using Vector4d = Vector4<double>;

template <typename Vector2>
typename Vector2::Scalar crossProduct2(const Vector2& v1, const Vector2& v2) {
    return (v1.x() * v2.y()) - (v1.y() * v2.x());
}

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Vector_0d3445ef_7a2f_4c97_bf41_fd6b1e54710c_h
