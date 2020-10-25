/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Scene_501ee1a7_47a6_4ed2_b9db_f31ba90ccc08_h
#define Flatland_Scene_501ee1a7_47a6_4ed2_b9db_f31ba90ccc08_h

#include "flatland/camera/camera.h"
#include "flatland/canvas/label.h"
#include "flatland/core/sampler.h"
#include "flatland/integrator/integrator.h"

FLATLAND_BEGIN_NAMESPACE

template <unsigned int Dimension, typename ScalarType>
class SceneType {
public:
    using Shape = ShapeType<Dimension, ScalarType>;
    using Ray = RayType<Dimension, ScalarType>;
    using MediumEvent = MediumEventType<Dimension, ScalarType>;
    using Camera = CameraType<Dimension, ScalarType>;
    using Integrator = IntegratorType<Dimension, ScalarType>;
    using Sampler = SamplerType<ScalarType>;
    using Label = LabelType2<ScalarType>;
    using Scalar = ScalarType;

	SceneType() = default;
    
    void addShape(ReferenceCounted<Shape> shape) {
        shapes_.push_back(shape);
    }
    std::vector<ReferenceCounted<Shape>> getShapes() const {
        return shapes_;
    }
    size_t getShapeCount() const { return shapes_.size(); }

    bool intersect(const Ray &ray, MediumEvent &me) const {
        me.t = std::numeric_limits<Scalar>::infinity();

        for (auto shape : shapes_) {
            MediumEvent shape_me;
            bool shape_hit = shape->intersect(ray, shape_me);

            if(shape_hit && shape_me.t < me.t) {
                me = shape_me;
            }
        }

        return me.t != std::numeric_limits<Scalar>::infinity();
    }

    void setCamera(ReferenceCounted<Camera> camera) {
        camera_ = camera;
    }
    ReferenceCounted<Camera> getCamera() const {
        return camera_;
    }

    void setIntegrator(ReferenceCounted<Integrator> integrator) {
        integrator_ = integrator;
    }
    ReferenceCounted<Integrator> getIntegrator() {
        return integrator_;
    }

    void addAnnotation(const Label& text)  {
        annotations_.push_back(text);
    }
    std::vector<Label> getAnnotations() {
        return annotations_;
    }
    size_t getAnnotationCount() {
        return annotations_.size();
    }

    void setSampler(ReferenceCounted<Sampler> sampler) {
        sampler_ = sampler;
    }
    ReferenceCounted<Sampler> getSampler() {
        return sampler_;
    }

private:
    std::vector<Label> annotations_;
	std::vector<ReferenceCounted<Shape>> shapes_;
	ReferenceCounted<Camera> camera_ = nullptr;
    ReferenceCounted<Integrator> integrator_;
    ReferenceCounted<Sampler> sampler_;
};

template <typename ScalarValue>
using Scene2 = SceneType<2, ScalarValue>;
template <typename ScalarValue>
using Scene3 = SceneType<3, ScalarValue>;

using Scene2f = Scene2<float>;
using Scene2d = Scene2<double>;
using Scene3f = Scene3<float>;
using Scene3d = Scene3<double>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Scene_501ee1a7_47a6_4ed2_b9db_f31ba90ccc08_h
