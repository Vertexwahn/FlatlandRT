/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Normal_c861abf5_ddbf_4bba_bd12_7cb61a99245c_h
#define Flatland_Normal_c861abf5_ddbf_4bba_bd12_7cb61a99245c_h

#include "flatland/core/namespace.h"

#include <Eigen/Core>
#include <Eigen/Dense>

FLATLAND_BEGIN_NAMESPACE

template<unsigned int Dimension, typename ScalarType>
struct NormalType : public Eigen::Matrix<ScalarType, Dimension, 1> {
    using Base = Eigen::Matrix<ScalarType, Dimension, 1>;

    NormalType() : Base() {}
    NormalType(const ScalarType x, const ScalarType y) : Base(x, y) {}
    NormalType(const ScalarType x, const ScalarType y, const ScalarType z) : Base(x, y, z) {}

    template <typename Derived>
    NormalType(const Eigen::MatrixBase<Derived>& src) : Base(src) {
    }

    using Base::operator=;
};

template <typename ScalarType>
using Normal2 = NormalType<2, ScalarType>;
template <typename ScalarType>
using Normal3 = NormalType<3, ScalarType>;

using Normal2i = Normal2<int>;
using Normal2f = Normal2<float>;
using Normal2d = Normal2<double>;
using Normal3i = Normal3<int>;
using Normal3f = Normal3<float>;
using Normal3d = Normal3<double>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Normal_c861abf5_ddbf_4bba_bd12_7cb61a99245c_h
