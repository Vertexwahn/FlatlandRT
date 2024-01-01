
/*
 *  SPDX-FileCopyrightText: Copyright 2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Imaging_Color_0cb9a6fe_978e_4062_95e9_07dfabe40508_h
#define De_Vertexwahn_Imaging_Color_0cb9a6fe_978e_4062_95e9_07dfabe40508_h

#include "math/util.h"

#include "core/namespace.h"

#include "fmt/core.h"

#include "Eigen/Core"
#include "Eigen/Dense"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template<typename ScalarType, unsigned int Dimension>
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

    [[nodiscard]]
    bool has_nans() const {
        for (size_t i = 0; i < Dimension; ++i) {
            if (std::isnan(this->coeff(i))) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]]
    std::string to_string() const {
        return fmt::format("{}, {}, {}", red(), green(), blue());
    }
};

template<typename ScalarType>
struct ColorType<ScalarType, 1> : public Eigen::Matrix<ScalarType, 1, 1> {
    using Base = Eigen::Matrix<ScalarType, 1, 1>;
    using Scalar = ScalarType;

    ColorType() : Base() {}

    explicit ColorType(const ScalarType grayvalue) : Base(grayvalue) {}

    template<typename Derived>
    ColorType(const Eigen::MatrixBase <Derived> &src) : Base(src) {
    }

    using Base::operator=;

    Scalar &grayvalue() {
        return Base::x();
    }

    const Scalar &grayvalue() const {
        return Base::x();
    }

    ColorType &clamp(const ScalarType min = ScalarType{0}, const ScalarType max = ScalarType{1}) {
        for (long int i = 0; i < Base::size(); ++i) {
            (*this)[i] = std::clamp((*this)[i], min, max);
        }

        return *this;
    }

    [[nodiscard]]
    bool has_nans() const {
        for (size_t i = 0; i < 1; ++i) {
            if (std::isnan(this->coeff(i))) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]]
    std::string to_string() const {
        return fmt::format("{}", grayvalue());
    }
};

template<typename ScalarType, unsigned int Dimension>
inline ColorType<ScalarType, Dimension> operator*(const ColorType<ScalarType, Dimension>& lhs, const ColorType<ScalarType, Dimension>& rhs)
{
    ColorType<ScalarType, Dimension> result;

    for (size_t i = 0; i < Dimension; ++i)
        result[i] = lhs[i] * rhs[i];

    return result;
}

template<typename ScalarType, unsigned int Dimension>
inline ColorType<ScalarType, Dimension> operator*(const ColorType<ScalarType, Dimension>& lhs, const ScalarType rhs)
{
    ColorType<ScalarType, Dimension> result;

    for (size_t i = 0; i < Dimension; ++i)
        result[i] = lhs[i] * rhs;

    return result;
}

template<typename ScalarType>
struct ColorConstants3 {
    using Scalar = ScalarType;
    static constexpr ColorType<ScalarType, 3> Black{Scalar{0.0}, Scalar{0.0}, Scalar{0.0}};
    static constexpr ColorType<ScalarType, 3> White{Scalar{1.0}, Scalar{1.0}, Scalar{1.0}};
};

using ColorConstants3f = ColorConstants3<float>;
using ColorConstants3d = ColorConstants3<double>;

template<typename ScalarType>
using Color1 = ColorType<ScalarType, 1>;
template<typename ScalarType>
using Color3 = ColorType<ScalarType, 3>;
template<typename ScalarType>
using Color4 = ColorType<ScalarType, 4>;

using Color1b = Color1<std::uint8_t>;
using Color1f = Color1<float>;
using Color1d = Color1<double>;

using Color3b = Color3<std::uint8_t>;
using Color3f = Color3<float>;
using Color3d = Color3<double>;

using Color4b = Color4<std::uint8_t>;
using Color4f = Color4<float>;
using Color4d = Color4<double>;

Color3f hot_to_cold_color_ramp(float v, float vmin, float vmax); // Todo: Move this to color_ramp.h

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Imaging_Color_0cb9a6fe_978e_4062_95e9_07dfabe40508_h
