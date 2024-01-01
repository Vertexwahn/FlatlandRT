/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Math_Normal_c861abf5_ddbf_4bba_bd12_7cb61a99245c_h
#define De_Vertexwahn_Math_Normal_c861abf5_ddbf_4bba_bd12_7cb61a99245c_h

#include "core/namespace.h"

#include "Eigen/Core"
#include "Eigen/Dense"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template<typename ScalarType, unsigned int Dimension>
struct NormalType : public Eigen::Matrix<ScalarType, Dimension, 1> {
    using Base = Eigen::Matrix<ScalarType, Dimension, 1>;

    NormalType() : Base() {}
    NormalType(const ScalarType x, const ScalarType y) : Base(x, y) {}
    NormalType(const ScalarType x, const ScalarType y, const ScalarType z) : Base(x, y, z) {}

    template <typename Derived>
    NormalType(const Eigen::MatrixBase<Derived>& src) : Base(src) {
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
using Normal2 = NormalType<ScalarType, 2>;
template <typename ScalarType>
using Normal3 = NormalType<ScalarType, 3>;

using Normal2i = Normal2<int>;
using Normal2f = Normal2<float>;
using Normal2d = Normal2<double>;
using Normal3i = Normal3<int>;
using Normal3f = Normal3<float>;
using Normal3d = Normal3<double>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Math_Normal_c861abf5_ddbf_4bba_bd12_7cb61a99245c_h
