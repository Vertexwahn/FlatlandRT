/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Math_Vector_0d3445ef_7a2f_4c97_bf41_fd6b1e54710c_h
#define De_Vertexwahn_Math_Vector_0d3445ef_7a2f_4c97_bf41_fd6b1e54710c_h

#include "core/namespace.h"

#include "Eigen/Core"
#include "Eigen/Dense"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template<typename ScalarType, unsigned int Dimension>
struct VectorType : public Eigen::Matrix<ScalarType, Dimension, 1> {
    using Base = Eigen::Matrix<ScalarType, Dimension, 1>;

    VectorType() : Base() {}
    VectorType(const ScalarType x, const ScalarType y) : Base(x,y) {}
    VectorType(const ScalarType x, const ScalarType y, const ScalarType z) : Base(x,y,z) {}
    VectorType(const ScalarType x, const ScalarType y, const ScalarType z, const ScalarType w) : Base(x,y,z,w) {}
    explicit VectorType(const ScalarType value) {
        *this = Base::Constant(value);
    }

    template <typename Derived>
    VectorType(const Eigen::MatrixBase<Derived>& src) : Base(src) {
    }

    using Base::operator=;

    bool has_nans() const {
        for (size_t i=0; i<Dimension; ++i) {
            if(std::isnan(this->coeff(i))) {
                return true;
            }
        }
        return false;
    }
};

template <typename ScalarType>
using Vector2 = VectorType<ScalarType, 2>;
template <typename ScalarType>
using Vector3 = VectorType<ScalarType, 3>;
template <typename ScalarType>
using Vector4 = VectorType<ScalarType, 4>;

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
typename Vector2::Scalar cross_product_2(const Vector2& v1, const Vector2& v2) {
    return (v1.x() * v2.y()) - (v1.y() * v2.x());
}

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Math_Vector_0d3445ef_7a2f_4c97_bf41_fd6b1e54710c_h
