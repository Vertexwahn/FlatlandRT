/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Integrator_74e1969c_9ae1_4c6c_b5f2_3b0545a65bd4_h
#define Flatland_Integrator_74e1969c_9ae1_4c6c_b5f2_3b0545a65bd4_h

#include "flatland/canvas/svg_canvas.h"
#include "flatland/core/color.h"
#include "flatland/core/logging.h"
#include "flatland/core/namespace.h"
#include "flatland/integrator/integrator.h"

FLATLAND_BEGIN_NAMESPACE

template <unsigned int Dimension, typename ScalarType>
class SceneType;

template <typename ScalarValue>
using Scene2 = SceneType<2, ScalarValue>;

using Scene2f = Scene2<float>;

template <unsigned int Dimension, typename ScalarType>
class IntegratorType {
public:
    using Ray = RayType<Dimension, ScalarType>;
    using Scene = SceneType<Dimension, ScalarType>;
    using SvgCanvas = SvgCanvasType<ScalarType>;

    IntegratorType(const PropertySet& ps) {
        max_depth = ps.getProperty("max_depth", 5);
    }

    virtual ~IntegratorType() {}

    void setCanvas(ReferenceCounted<SvgCanvas> canvas) {
        canvas_ = canvas;
    }

    void render(const Scene* scene) {
        assert(scene);
        assert(canvas_);

        LOG(INFO) << "Begin rendering.";

        for (auto shape : scene->getShapes()) {
            canvas_->add(shape.get());
        }

        auto camera = scene->getCamera();
        assert(camera);

        Ray ray = camera->generateRay();

        trace(scene, ray, 1);
    }

    virtual Color3<ScalarType> trace(
            const Scene *scene,
            Ray &ray,
            const int depth) const = 0;

protected:
    ReferenceCounted<SvgCanvas> canvas_ = nullptr;
    int max_depth = 0;
};

using Integrator2f = IntegratorType<2, float>;
using Integrator2d = IntegratorType<2, double>;
using Integrator3f = IntegratorType<3, float>;
using Integrator3d = IntegratorType<3, double>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Integrator_74e1969c_9ae1_4c6c_b5f2_3b0545a65bd4_h
