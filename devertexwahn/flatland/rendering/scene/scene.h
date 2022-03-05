/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Scene_501ee1a7_47a6_4ed2_b9db_f31ba90ccc08_h
#define Flatland_Scene_501ee1a7_47a6_4ed2_b9db_f31ba90ccc08_h

#include "flatland/rendering/canvas/label.h"
#include "flatland/rendering/integrator/integrator.h"
#include "flatland/rendering/intersector/intersector.h"
#include "flatland/rendering/scene/shape/shape.h"
#include "flatland/rendering/sampler.h"
#include "flatland/rendering/sensor/sensor.h"

#include <cassert>

FLATLAND_BEGIN_NAMESPACE

template<unsigned int Dimension, typename ScalarType>
class SceneType {
public:
    using Shape = ShapeType<Dimension, ScalarType>;
    using Ray = RayType<Dimension, ScalarType>;
    using MediumEvent = MediumEventType<Dimension, ScalarType>;
    using Sensor = SensorType<Dimension, ScalarType>;
    using Integrator = IntegratorType<Dimension, ScalarType>;
    using Sampler = SamplerType<ScalarType>;
    using Label = LabelType2<ScalarType>;
    using Scalar = ScalarType;
    using Intersector = IntersectorType<Dimension, ScalarType>;

    SceneType() = default;

    ~SceneType() {}

    void add_shape(ReferenceCounted<Shape> shape) {
        shapes_.push_back(shape);
    }

    std::vector<ReferenceCounted<Shape>> shapes() const {
        return shapes_;
    }

    size_t shape_count() const { return shapes_.size(); }

    bool intersect(const Ray &ray, MediumEvent &me) const {
        assert(intersector_);
        return intersector_->intersect(ray, me);
    }

    bool intersect(const Ray &ray) const {
        MediumEvent me;
        return intersector_->intersect(ray, me);
    }

    void set_sensor(ReferenceCounted<Sensor> sensor) {
        sensor_ = sensor;
    }

    ReferenceCounted<Sensor> sensor() const {
        return sensor_;
    }

    void set_integrator(ReferenceCounted<Integrator> integrator) {
        integrator_ = integrator;
    }

    ReferenceCounted<Integrator> integrator() const {
        return integrator_;
    }

    void add_annotation(const Label &text) {
        annotations_.push_back(text);
    }

    std::vector<Label> annotations() {
        return annotations_;
    }

    size_t annotation_count() {
        return annotations_.size();
    }

    void set_sampler(ReferenceCounted<Sampler> sampler) {
        sampler_ = sampler;
    }

    ReferenceCounted<Sampler> sampler() const {
        return sampler_;
    }

    void set_intersector(ReferenceCounted<Intersector> intersector) {
        intersector_ = intersector;
    }

    bool has_intersector() const {
        return intersector_ != nullptr;
    }

    ReferenceCounted<Intersector> intersector() {
        return intersector_;
    }

private:
    std::vector<Label> annotations_;
    std::vector<ReferenceCounted<Shape>> shapes_;
    ReferenceCounted<Sensor> sensor_;
    ReferenceCounted<Integrator> integrator_;
    ReferenceCounted<Sampler> sampler_;
    ReferenceCounted<Intersector> intersector_;
};

template<typename ScalarType>
using Scene2 = SceneType<2, ScalarType>;
template<typename ScalarType>
using Scene3 = SceneType<3, ScalarType>;

using Scene2f = Scene2<float>;
using Scene2d = Scene2<double>;

using Scene3f = Scene3<float>;
using Scene3d = Scene3<double>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Scene_501ee1a7_47a6_4ed2_b9db_f31ba90ccc08_h
