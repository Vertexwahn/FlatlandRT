/*
 *  SPDX-FileCopyrightText: Copyright 2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Imaging_Color_0cb9a6fe_978e_4062_95e9_07dfabe40508_h
#define De_Vertexwahn_Imaging_Color_0cb9a6fe_978e_4062_95e9_07dfabe40508_h

#include "math/util.hpp"

#include "core/namespace.hpp"

#include "fmt/core.h"

#include "Eigen/Core"
#include "Eigen/Dense"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template<typename ScalarType, unsigned int Dimension>
struct ColorTypeRGB : public Eigen::Matrix<ScalarType, Dimension, 1> {
    using Base = Eigen::Matrix<ScalarType, Dimension, 1>;
    using Scalar = ScalarType;

    ColorTypeRGB() : Base() {}

    ColorTypeRGB(const ScalarType red, const ScalarType green, const ScalarType blue) : Base(red, green, blue) {}

    ColorTypeRGB(const ScalarType red, const ScalarType green, const ScalarType blue, const ScalarType alpha) : Base(
            red, green, blue, alpha) {}

    explicit ColorTypeRGB(const ScalarType value) {
        *this = Base::Constant(value);
    }

    template<typename Derived>
    ColorTypeRGB(const Eigen::MatrixBase <Derived> &src) : Base(src) {
    }

    using Base::operator=;

    [[nodiscard]]
    Scalar &red() {
        static_assert(Dimension > 0);
        return Base::x();
    }

    [[nodiscard]]
    Scalar &green() {
        static_assert(Dimension > 1);
        return Base::y();
    }

    [[nodiscard]]
    Scalar &blue() {
        static_assert(Dimension > 2);
        return Base::z();
    }

    [[nodiscard]]
    Scalar &alpha() {
        static_assert(Dimension > 3);
        return (*this)[3];
    }

    [[nodiscard]]
    const Scalar &red() const {
        static_assert(Dimension > 0);
        return Base::x();
    }

    [[nodiscard]]
    const Scalar &green() const {
        static_assert(Dimension > 1);
        return Base::y();
    }

    [[nodiscard]]
    const Scalar &blue() const {
        static_assert(Dimension > 2);
        return Base::z();
    }

    [[nodiscard]]
    const Scalar &alpha() const {
        static_assert(Dimension > 3);
        return (*this)[3];
    }

    [[nodiscard]]
    const ColorTypeRGB<Scalar,3> rgb() const {
        return ColorTypeRGB<Scalar,3>{red(), green(), blue()};
    }

    ColorTypeRGB &clamp(const ScalarType min = ScalarType{0}, const ScalarType max = ScalarType{1}) {
        for (long int i = 0; i < Base::size(); ++i) {
            (*this)[i] = std::clamp((*this)[i], min, max);
        }

        return *this;
    }

    ColorTypeRGB& clamp_negative() {
        for (long int i = 0; i < Base::size(); ++i) {
            (*this)[i] = std::max(Scalar{0}, (*this)[i]);
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
struct ColorTypeRGB<ScalarType, 1> : public Eigen::Matrix<ScalarType, 1, 1> {
    using Base = Eigen::Matrix<ScalarType, 1, 1>;
    using Scalar = ScalarType;

    ColorTypeRGB() : Base() {}

    explicit ColorTypeRGB(const ScalarType grayvalue) : Base(grayvalue) {}

    template<typename Derived>
    ColorTypeRGB(const Eigen::MatrixBase <Derived> &src) : Base(src) {
    }

    using Base::operator=;

    Scalar &grayvalue() {
        return Base::x();
    }

    const Scalar &grayvalue() const {
        return Base::x();
    }

    ColorTypeRGB &clamp(const ScalarType min = ScalarType{0}, const ScalarType max = ScalarType{1}) {
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
inline ColorTypeRGB<ScalarType, Dimension> operator*(const ColorTypeRGB<ScalarType, Dimension>& lhs, const ColorTypeRGB<ScalarType, Dimension>& rhs)
{
    ColorTypeRGB<ScalarType, Dimension> result;

    for (size_t i = 0; i < Dimension; ++i)
        result[i] = lhs[i] * rhs[i];

    return result;
}

template<typename ScalarType, unsigned int Dimension>
inline ColorTypeRGB<ScalarType, Dimension> operator*(const ColorTypeRGB<ScalarType, Dimension>& lhs, const ScalarType rhs)
{
    ColorTypeRGB<ScalarType, Dimension> result;

    for (size_t i = 0; i < Dimension; ++i)
        result[i] = lhs[i] * rhs;

    return result;
}

template<typename ScalarType>
struct ColorRGBConstants3 {
    using Scalar = ScalarType;
    static constexpr ColorTypeRGB<ScalarType, 3> Black{Scalar{0.0}, Scalar{0.0}, Scalar{0.0}};
    static constexpr ColorTypeRGB<ScalarType, 3> White{Scalar{1.0}, Scalar{1.0}, Scalar{1.0}};
};

using ColorRGBConstants3f = ColorRGBConstants3<float>;
using ColorRGBConstants3d = ColorRGBConstants3<double>;

template<typename ScalarType>
using ColorGrayscale = ColorTypeRGB<ScalarType, 1>;
template<typename ScalarType>
using ColorRGB = ColorTypeRGB<ScalarType, 3>;
template<typename ScalarType>
using ColorRGBA = ColorTypeRGB<ScalarType, 4>;

using Color1b = ColorGrayscale<std::uint8_t>;
using Color1f = ColorGrayscale<float>;
using Color1d = ColorGrayscale<double>;

using ColorRGB3b = ColorRGB<std::uint8_t>;
using ColorRGB3f = ColorRGB<float>;
using ColorRGB3d = ColorRGB<double>;

using ColorRGBA4b = ColorRGBA<std::uint8_t>;
using ColorRGBA4f = ColorRGBA<float>;
using ColorRGBA4d = ColorRGBA<double>;

ColorRGB3f hot_to_cold_color_ramp(float v, float vmin, float vmax); // Todo: Move this to color_ramp.h

template<typename ScalarType>
struct ColorXYZ  : public Eigen::Matrix<ScalarType, 3, 1> {
    using Base = Eigen::Matrix<ScalarType, 3, 1>;
    using Scalar = ScalarType;

    ColorXYZ() : Base() {}
    ColorXYZ(const ScalarType x, const ScalarType y, const ScalarType z) : Base(x, y, z) {}
};

using ColorXYZ3f = ColorXYZ<float>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Imaging_Color_0cb9a6fe_978e_4062_95e9_07dfabe40508_h
