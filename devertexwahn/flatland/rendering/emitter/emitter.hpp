/*
 *  SPDX-FileCopyrightText: Copyright 2025 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Okapi_Emitter_03bdb00d_17f5_4478_a82b_3016f2a5e0e7_hpp
#define De_Vertexwahn_Okapi_Emitter_03bdb00d_17f5_4478_a82b_3016f2a5e0e7_hpp

#include "core/namespace.hpp"
#include "core/object.hpp"
#include "core/property_set.hpp"
#include "math/ray.hpp"
#include "imaging/color.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template <typename ScalarType, unsigned int Dimension>
class EmitterType : public Object {
public:
    using Ray = RayType<ScalarType, Dimension>;
    using Spectrum = ColorTypeRGB<ScalarType, 3>;

    explicit EmitterType() = default;
    virtual ~EmitterType() {}

    virtual Spectrum evaluate() const = 0;

    virtual Spectrum evaluate(const Ray &ray) const {
        return evaluate();
    }

    [[nodiscard]]
    virtual std::string to_string() const override {
        return "Emitter";
    }
};

using EmitterType2f = EmitterType<float, 2>;
using EmitterType2d = EmitterType<double, 2>;
using EmitterType3f = EmitterType<float, 3>;
using EmitterType3d = EmitterType<double, 3>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Okapi_Emitter_03bdb00d_17f5_4478_a82b_3016f2a5e0e7_hpp
