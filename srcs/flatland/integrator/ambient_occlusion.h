/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_AmbientOcclusion_2ea71cc7_ef11_4e85_bb62_4f81a780fd35_h
#define Flatland_AmbientOcclusion_2ea71cc7_ef11_4e85_bb62_4f81a780fd35_h

#include "flatland/integrator/integrator.h"

FLATLAND_BEGIN_NAMESPACE

template<unsigned int Dimension, typename ScalarType>
class AmbientOcclusion : public IntegratorType<Dimension, ScalarType> {
public:
    using Base = IntegratorType<2, ScalarType>;
    using Color = ColorType<3, ScalarType>;
    using Scalar = ScalarType;

    AmbientOcclusion(const PropertySet& ps) : IntegratorType<2, ScalarType>(ps) {
    }
    virtual ~AmbientOcclusion() {
    }

    Color trace(
            const Scene2<ScalarType> *scene,
            Ray2<ScalarType> &ray,
            const int depth) const override {
        return Color{Scalar{1.0f}, Scalar{0.0f}, Scalar{0.0f}};
    }
};

using AmbientOcclusion2f = AmbientOcclusion<2, float>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_AmbientOcclusion_2ea71cc7_ef11_4e85_bb62_4f81a780fd35_h
