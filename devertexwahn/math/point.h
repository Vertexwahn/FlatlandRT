/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Math_Point_8f63f4af_19a9_4406_8d7a_a0db4f946277_h
#define De_Vertexwahn_Math_Point_8f63f4af_19a9_4406_8d7a_a0db4f946277_h

#include "core/namespace.h"

#include "Eigen/Core"
#include "Eigen/Dense"

#include <vector>

DE_VERTEXWAHN_BEGIN_NAMESPACE

template<typename ScalarType, unsigned int Dimension>
struct PointType : public Eigen::Matrix<ScalarType, Dimension, 1> {
    using Base = Eigen::Matrix<ScalarType, Dimension, 1>;

    PointType() : Base() {}
    PointType(const ScalarType x, const ScalarType y) : Base(x, y) {}
    PointType(const ScalarType x, const ScalarType y, const ScalarType z) : Base(x, y, z) {}
    PointType(const ScalarType x, const ScalarType y, const ScalarType z, const ScalarType w) : Base(x, y, z, w) {}
    explicit PointType(const ScalarType value) {
        *this = Base::Constant(value);
    }

    template <typename Derived>
    PointType(const Eigen::MatrixBase<Derived>& src) : Base(src) {
    }

    PointType<ScalarType, 3> xyz() const {
        return PointType<ScalarType, 3>(Base::x(), Base::y(), Base::z());
    }

    PointType<ScalarType, 2> zy() const {
        return PointType<ScalarType, 2>(Base::z(), Base::y());
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
using Point2 = PointType<ScalarType, 2>;
template <typename ScalarType>
using Point3 = PointType<ScalarType, 3>;
template <typename ScalarType>
using Point4 = PointType<ScalarType, 4>;

using Point2i = Point2<int>;
using Point2f = Point2<float>;
using Point2d = Point2<double>;

using Point3i = Point3<int>;
using Point3f = Point3<float>;
using Point3d = Point3<double>;

using Point4i = Point4<int>;
using Point4f = Point4<float>;
using Point4d = Point4<double>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Math_Point_8f63f4af_19a9_4406_8d7a_a0db4f946277_h
