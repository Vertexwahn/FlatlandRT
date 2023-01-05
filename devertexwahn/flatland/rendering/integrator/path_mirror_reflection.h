/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_Rendering_Intersector_f47251b4_9a7b_4321_a54f_cdf570fa8dd6_h
#define De_Vertexwahn_Flatland_Rendering_Intersector_f47251b4_9a7b_4321_a54f_cdf570fa8dd6_h

#include "flatland/rendering/integrator/integrator.h"
#include "math/refract.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

// from pbrt v3 with modifications
template<typename ScalarType>
Vector2<ScalarType> reflect(const Vector2<ScalarType> &wo, const Normal2<ScalarType> &normal) {
    return -wo + 2 * wo.dot(normal) * normal;
}

// This integrator follows a sensor ray and only expects at each surface interaction a mirror reflection
template<typename ScalarType>
class PathMirrorReflection2 : public IntegratorType<2, ScalarType> {
public:
    using Base = IntegratorType<2, ScalarType>;
    using Color = ColorType<3, ScalarType>;
    using Scalar = ScalarType;
    using Sampler = SamplerType<ScalarType>;

    PathMirrorReflection2(const PropertySet &ps) : IntegratorType<2, ScalarType>(ps) {
        max_depth = ps.get_property("max_depth", 5);
    }

    virtual ~PathMirrorReflection2() {
    }

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
            Vector2<ScalarType> reflectedDirection = reflect<ScalarType>(wi, me.geo_frame.normal.normalized());

            Ray2f reflectedRay(me.p + reflectedDirection * ScalarType{0.01}, reflectedDirection, ScalarType{0.0}, ScalarType{20000.0});

            trace(scene, sampler, reflectedRay, depth+1);
        }
        else {
            ray.max_t = 100.f;
        }

        Base::canvas_->add(ray);

        return Color{Scalar{0.0}, Scalar{0.0}, Scalar{0.0}};
    }

private:
    int max_depth = 0;
};

using PathMirrorReflection2f = PathMirrorReflection2<float>;
using PathMirrorReflection2d = PathMirrorReflection2<double>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Rendering_Intersector_f47251b4_9a7b_4321_a54f_cdf570fa8dd6_h
