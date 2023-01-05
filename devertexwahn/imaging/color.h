/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Imaging_Color_0cb9a6fe_978e_4062_95e9_07dfabe40508_h
#define De_Vertexwahn_Imaging_Color_0cb9a6fe_978e_4062_95e9_07dfabe40508_h

#include "core/namespace.h"
#include "math/util.h"

#include "fmt/core.h"
#include "Eigen/Core"
#include "Eigen/Dense"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template<unsigned int Dimension, typename ScalarType>
struct ColorType : public Eigen::Matrix<ScalarType, Dimension, 1> {
    using Base = Eigen::Matrix<ScalarType, Dimension, 1>;
    using Scalar = ScalarType;

    ColorType() : Base() {}

    ColorType(const ScalarType red, const ScalarType green, const ScalarType blue) : Base(red, green, blue) {}

    ColorType(const ScalarType red, const ScalarType green, const ScalarType blue, const ScalarType alpha) : Base(
            red, green, blue, alpha) {}

    explicit ColorType(const ScalarType value) {
        *this = Base::Constant(value);
    }

    template<typename Derived>
    ColorType(const Eigen::MatrixBase <Derived> &src) : Base(src) {
    }

    using Base::operator=;

    Scalar &red() {
        static_assert(Dimension > 0);
        return Base::x();
    }

    Scalar &green() {
        static_assert(Dimension > 1);
        return Base::y();
    }

    Scalar &blue() {
        static_assert(Dimension > 2);
        return Base::z();
    }

    const Scalar &red() const {
        static_assert(Dimension > 0);
        return Base::x();
    }

    const Scalar &green() const {
        static_assert(Dimension > 1);
        return Base::y();
    }

    const Scalar &blue() const {
        static_assert(Dimension > 2);
        return Base::z();
    }

    ColorType &clamp(const ScalarType min = ScalarType{0}, const ScalarType max = ScalarType{1}) {
        for (long int i = 0; i < Base::size(); ++i) {
            (*this)[i] = std::clamp((*this)[i], min, max);
        }

        return *this;
    }

    bool has_nans() const {
        for (size_t i = 0; i < Dimension; ++i) {
            if (std::isnan(this->coeff(i))) {
                return true;
            }
        }
        return false;
    }

    std::string to_string() const {
        return fmt::format("{}, {}, {}", red(), green(), blue());
    }
};

template<unsigned int Dimension, typename ScalarType>
inline ColorType<Dimension, ScalarType> operator*(const ColorType<Dimension, ScalarType>& lhs, const ColorType<Dimension, ScalarType>& rhs)
{
    ColorType<Dimension, ScalarType> result;

    for (size_t i = 0; i < Dimension; ++i)
        result[i] = lhs[i] * rhs[i];

    return result;
}

template<typename ScalarType>
struct ColorConstants3 {
    using Scalar = ScalarType;
    static constexpr ColorType<3, ScalarType> Black{Scalar{0.0}, Scalar{0.0}, Scalar{0.0}};
    static constexpr ColorType<3, ScalarType> White{Scalar{1.0}, Scalar{1.0}, Scalar{1.0}};
};

using ColorConstants3f = ColorConstants3<float>;
using ColorConstants3d = ColorConstants3<double>;

template<typename ScalarType>
using Color3 = ColorType<3, ScalarType>;
template<typename ScalarType>
using Color4 = ColorType<4, ScalarType>;

using Color3f = Color3<float>;
using Color3d = Color3<double>;
using Color3b = Color3<std::uint8_t>;

using Color4f = Color4<float>;
using Color4d = Color4<double>;
using Color4b = Color4<std::uint8_t>;

Color3f hot_to_cold_color_ramp(float v, float vmin, float vmax);

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Imaging_Color_0cb9a6fe_978e_4062_95e9_07dfabe40508_h
