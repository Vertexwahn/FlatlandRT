/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_Integrator_74e1969c_9ae1_4c6c_b5f2_3b0545a65bd4_h
#define De_Vertexwahn_Flatland_Integrator_74e1969c_9ae1_4c6c_b5f2_3b0545a65bd4_h

#include "core/object.h"
#include "imaging/color.h"
#include "flatland/rendering/canvas/svg_canvas.h"
#include "flatland/rendering/sampler.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template <unsigned int Dimension, typename ScalarType>
class SceneType;

template <typename ScalarType>
using Scene2 = SceneType<2, ScalarType>;

using Scene2f = Scene2<float>;

template <unsigned int Dimension, typename ScalarType>
class IntegratorType : public Object {
public:
    using Ray = RayType<Dimension, ScalarType>;
    using Scene = SceneType<Dimension, ScalarType>;
    using SvgCanvas = SvgCanvasType<ScalarType>;
    using Sampler = SamplerType<ScalarType>;

    IntegratorType(const PropertySet& ps) {
    }

    virtual ~IntegratorType() {}

    virtual Color3<ScalarType> trace(
            const Scene *scene,
            Sampler* sampler,
            Ray &ray,
            const int depth,
            ScalarType *aovs = nullptr) const = 0;
};

template <typename ScalarType>
class IntegratorType<2, ScalarType> : public Object {
public:
    using Ray = RayType<2, ScalarType>;
    using Scene = SceneType<2, ScalarType>;
    using SvgCanvas = SvgCanvasType<ScalarType>;
    using Sampler = SamplerType<ScalarType>;

    IntegratorType(const PropertySet& ps) {
    }

    virtual ~IntegratorType() {}

    void set_canvas(ReferenceCounted<SvgCanvas> canvas) {
        canvas_ = canvas;
    }

    virtual Color3<ScalarType> trace(
            const Scene *scene,
            Sampler* sampler,
            Ray &ray,
            const int depth) const = 0;

protected:
    ReferenceCounted<SvgCanvas> canvas_ = nullptr;
};

using Integrator2f = IntegratorType<2, float>;
using Integrator2d = IntegratorType<2, double>;
using Integrator3f = IntegratorType<3, float>;
using Integrator3d = IntegratorType<3, double>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Integrator_74e1969c_9ae1_4c6c_b5f2_3b0545a65bd4_h
