/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_Integrator_74e1969c_9ae1_4c6c_b5f2_3b0545a65bd4_h
#define De_Vertexwahn_Flatland_Integrator_74e1969c_9ae1_4c6c_b5f2_3b0545a65bd4_h

#include "core/object.h"
#include "flatland/rendering/canvas/svg_canvas.h"
#include "flatland/rendering/sampler.h"
#include "imaging/color.h"

#include <mutex>

DE_VERTEXWAHN_BEGIN_NAMESPACE

template <typename ScalarType, unsigned int Dimension>
class SceneType;

template <typename ScalarType>
using Scene2 = SceneType<ScalarType, 2>;

using Scene2f = Scene2<float>;

template <typename ScalarType, unsigned int Dimension>
class DebugReverseLightPathType {
public:
    using MediumEvent = MediumEventType<ScalarType, Dimension>;

    void clear() {
        std::scoped_lock lock{mutex};
        medium_events.clear();
    }
    void add_medium_event(const MediumEvent& me) {
        std::scoped_lock lock{mutex};
        medium_events.push_back(me);
    }

    std::mutex mutex;
    using Point = PointType<ScalarType, Dimension>;
    std::vector<MediumEvent> medium_events;
};

template <typename ScalarType, unsigned int Dimension>
class IntegratorType : public Object {
public:
    using Ray = RayType<ScalarType, Dimension>;
    using Scene = SceneType<ScalarType, Dimension>;
    using SvgCanvas = SvgCanvasType<ScalarType>;
    using Sampler = SamplerType<ScalarType>;
    using DebugReverseLightPath = DebugReverseLightPathType<ScalarType, Dimension>;

    IntegratorType(const PropertySet& ps) {
    }

    virtual ~IntegratorType() {}

    virtual std::vector<std::string> aov_names() const {
        return { };
    }

    virtual Color3<ScalarType> trace(
            const Scene *scene,
            Sampler* sampler,
            Ray &ray,
            const int depth,
            ScalarType *aovs = nullptr) const = 0;

    mutable DebugReverseLightPath drl_;
};

template <typename ScalarType>
class IntegratorType<ScalarType, 2> : public Object {
public:
    using Ray = RayType<ScalarType, 2>;
    using Scene = SceneType<ScalarType, 2>;
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

using Integrator2f = IntegratorType<float, 2>;
using Integrator2d = IntegratorType<double, 2>;
using Integrator3f = IntegratorType<float, 3>;
using Integrator3d = IntegratorType<double, 3>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Integrator_74e1969c_9ae1_4c6c_b5f2_3b0545a65bd4_h
