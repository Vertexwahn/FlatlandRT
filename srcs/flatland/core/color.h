/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Color_0cb9a6fe_978e_4062_95e9_07dfabe40508_h
#define Flatland_Color_0cb9a6fe_978e_4062_95e9_07dfabe40508_h

#include "flatland/core/namespace.h"

#include <Eigen/Core>
#include <Eigen/Dense>

FLATLAND_BEGIN_NAMESPACE

template<unsigned int Dimension, typename ScalarType>
struct ColorType : public Eigen::Matrix<ScalarType, Dimension, 1> {
    using Base = Eigen::Matrix<ScalarType, Dimension, 1>;
    using Scalar = ScalarType;

    ColorType() : Base() {}
    ColorType(const ScalarType red, const ScalarType green, const ScalarType blue) : Base(red, green, blue) {}
    ColorType(const ScalarType red, const ScalarType green, const ScalarType blue, const ScalarType alpha) : Base(red, green, blue, alpha) {}

    template <typename Derived>
    ColorType(const Eigen::MatrixBase<Derived>& src) : Base(src) {
    }

    const Scalar& red() const {
        static_assert(Dimension > 0);
        return Base::x();
    }

    const Scalar& green() const {
        static_assert(Dimension > 1);
        return Base::y();
    }

    const Scalar& blue() const {
        static_assert(Dimension > 2);
        return Base::z();
    }

    using Base::operator=;
};

template <typename ScalarType>
struct ColorConstants3 {
    using Scalar = ScalarType;
    static constexpr ColorType<3, ScalarType> Black{Scalar{0.0},Scalar{0.0},Scalar{0.0}};
};

using ColorConstants3f = ColorConstants3<float>;
using ColorConstants3d = ColorConstants3<double>;

template <typename ScalarType>
using Color3 = ColorType<3, ScalarType>;
template <typename ScalarType>
using Color4 = ColorType<4, ScalarType>;

using Color3f = Color3<float>;
using Color3d = Color3<double>;
using Color4f = Color3<float>;
using Color4d = Color3<double>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Color_0cb9a6fe_978e_4062_95e9_07dfabe40508_h
