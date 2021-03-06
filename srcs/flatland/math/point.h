/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Point_8f63f4af_19a9_4406_8d7a_a0db4f946277_h
#define Flatland_Point_8f63f4af_19a9_4406_8d7a_a0db4f946277_h

#include "flatland/core/namespace.h"

#include <Eigen/Core>
#include <Eigen/Dense>

#include <vector>

FLATLAND_BEGIN_NAMESPACE

template<unsigned int Dimension, typename ScalarType>
struct PointType : public Eigen::Matrix<ScalarType, Dimension, 1> {
    using Base = Eigen::Matrix<ScalarType, Dimension, 1>;

    PointType() : Base() {}
    PointType(const ScalarType x, const ScalarType y) : Base(x, y) {}
    PointType(const ScalarType x, const ScalarType y, const ScalarType z) : Base(x, y, z) {}
    PointType(const ScalarType x, const ScalarType y, const ScalarType z, const ScalarType w) : Base(x, y, z, w) {}

    template <typename Derived>
    PointType(const Eigen::MatrixBase<Derived>& src) : Base(src) {
    }

    using Base::operator=;
};

template <typename ScalarType>
using Point2 = PointType<2, ScalarType>;
template <typename ScalarType>
using Point3 = PointType<3, ScalarType>;
template <typename ScalarType>
using Point4 = PointType<4, ScalarType>;

using Point2i = Point2<int>;
using Point2f = Point2<float>;
using Point2d = Point2<double>;
using Point3i = Point3<int>;
using Point3f = Point3<float>;
using Point3d = Point3<double>;
using Point4i = Point4<int>;
using Point4f = Point4<float>;
using Point4d = Point4<double>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland__Point_8f63f4af_19a9_4406_8d7a_a0db4f946277_h
