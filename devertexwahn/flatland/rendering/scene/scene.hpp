/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_Scene_501ee1a7_47a6_4ed2_b9db_f31ba90ccc08_h
#define De_Vertexwahn_Flatland_Scene_501ee1a7_47a6_4ed2_b9db_f31ba90ccc08_h

#include "flatland/rendering/canvas/label.hpp"
#include "flatland/rendering/emitter/emitter.hpp"
#include "flatland/rendering/integrator/integrator.hpp"
#include "flatland/rendering/intersector/intersector.hpp"
#include "flatland/rendering/sampler.hpp"
#include "flatland/rendering/shape/shape.hpp"
#include "flatland/rendering/sensor/sensor.hpp"

#include <cassert>

DE_VERTEXWAHN_BEGIN_NAMESPACE

template<typename ScalarType, unsigned int Dimension>
class SceneType {
public:
    using Shape = ShapeType<ScalarType, Dimension>;
    using Ray = RayType<ScalarType, Dimension>;
    using MediumEvent = MediumEventType<ScalarType, Dimension>;
    using Sensor = SensorType<ScalarType, Dimension>;
    using Integrator = IntegratorType<ScalarType, Dimension>;
    using Sampler = SamplerType<ScalarType>;
    using Label = LabelType2<ScalarType>;
    using Scalar = ScalarType;
    using Intersector = IntersectorType<ScalarType, Dimension>;
    using Emitter = EmitterType<ScalarType, Dimension>;

    SceneType() = default;

    ~SceneType() {}

    void add_shape(ReferenceCounted<Shape> shape) {
        shapes_.push_back(shape);
    }

    std::vector<ReferenceCounted<Shape>> shapes() const {
        return shapes_;
    }

    [[nodiscard]]
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

    [[nodiscard]]
    ReferenceCounted<Integrator> integrator() const {
        return integrator_;
    }

    void add_annotation(const Label &text) {
        annotations_.push_back(text);
    }

    [[nodiscard]]
    std::vector<Label> annotations() {
        return annotations_;
    }

    [[nodiscard]]
    size_t annotation_count() {
        return annotations_.size();
    }

    void set_sampler(ReferenceCounted<Sampler> sampler) {
        sampler_ = sampler;
    }

    [[nodiscard]]
    ReferenceCounted<Sampler> sampler() const {
        return sampler_;
    }

    [[nodiscard]]
    ReferenceCounted<Sampler> sampler() {
        return sampler_;
    }

    void set_intersector(ReferenceCounted<Intersector> intersector) {
        intersector_ = intersector;
    }

    [[nodiscard]]
    bool has_intersector() const {
        return intersector_ != nullptr;
    }

    [[nodiscard]]
    ReferenceCounted<Intersector> intersector() const {
        return intersector_;
    }

    [[nodiscard]]
    bool has_environment_emitter() const {
        return environment_emitter_ != nullptr;
    }

    void set_environment(ReferenceCounted<Emitter> environment_environment) {
        environment_emitter_ = environment_environment;
    }

    [[nodiscard]]
    ReferenceCounted<Emitter> environment_emitter() const {
        return environment_emitter_;
    }

private:
    std::vector<Label> annotations_;
    std::vector<ReferenceCounted<Shape>> shapes_;
    ReferenceCounted<Sensor> sensor_;
    ReferenceCounted<Integrator> integrator_;
    ReferenceCounted<Sampler> sampler_;
    ReferenceCounted<Intersector> intersector_;
    ReferenceCounted<Emitter> environment_emitter_;
};

template<typename ScalarType>
using Scene2 = SceneType<ScalarType, 2>;
template<typename ScalarType>
using Scene3 = SceneType<ScalarType, 3>;

using Scene2f = Scene2<float>;
using Scene2d = Scene2<double>;

using Scene3f = Scene3<float>;
using Scene3d = Scene3<double>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Scene_501ee1a7_47a6_4ed2_b9db_f31ba90ccc08_h
