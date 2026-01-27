/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_Rendering_Intersector_f47251b4_9a7b_4321_a54f_cdf570fa8dd6_h
#define De_Vertexwahn_Flatland_Rendering_Intersector_f47251b4_9a7b_4321_a54f_cdf570fa8dd6_h

#include "flatland/rendering/integrator/integrator.hpp"
#include "flatland/rendering/scene/scene.hpp"

#include "math/refract.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

// from pbrt v3 with modifications
template<typename ScalarType>
Vector2<ScalarType> reflect(const Vector2<ScalarType> &wo, const Normal2<ScalarType> &normal) {
    return -wo + ScalarType{2} * wo.dot(normal) * normal;
}

// This integrator follows a sensor ray and only expects at each surface interaction a mirror reflection
template<typename ScalarType>
class PathMirrorReflection2 final : public IntegratorType<ScalarType, 2> {
public:
    using Base = IntegratorType<ScalarType, 2>;
    using Color = ColorTypeRGB<ScalarType, 3>;
    using Scalar = ScalarType;
    using Sampler = SamplerType<ScalarType>;

    explicit PathMirrorReflection2(const PropertySet &ps) : IntegratorType<ScalarType, 2>(ps) {
        max_depth = ps.get_property("max_depth", 5);
    }

    virtual ~PathMirrorReflection2() = default;

    Color trace(
            const Scene2<ScalarType> *scene,
            Sampler *sampler,
            Ray2<ScalarType> &ray,
            const int depth) const override {
        if(depth > max_depth)
            return Color{Scalar{0.0}, Scalar{0.0}, Scalar{0.0}};

        MediumEvent2<ScalarType> me;
        bool hit = scene->intersect(ray, me);

        if(hit) {
            ray.max_t = me.t;

            // compute reflection
            Vector2<ScalarType> wi = -ray.direction;
            wi.normalize();

            // inside or outside?
            Vector2<ScalarType> reflected_direction = reflect<ScalarType>(wi, me.geo_frame.n.normalized());

            Ray2f reflectedRay(me.p + reflected_direction * ScalarType{0.01}, reflected_direction, ScalarType{0.0}, ScalarType{20000.0});

            trace(scene, sampler, reflectedRay, depth+1);
        }
        else {
            ray.max_t = 100.f;
        }

        Base::canvas_->add(ray);

        return Color{Scalar{0.0}, Scalar{0.0}, Scalar{0.0}};
    }

private:
    int max_depth{};
};

using PathMirrorReflection2f = PathMirrorReflection2<float>;
using PathMirrorReflection2d = PathMirrorReflection2<double>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Rendering_Intersector_f47251b4_9a7b_4321_a54f_cdf570fa8dd6_h
