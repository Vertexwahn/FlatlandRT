/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_PathMirrorReflection_747251b4_9a7b_4321_a54f_cdf570fa8dd6_h
#define Flatland_PathMirrorReflection_747251b4_9a7b_4321_a54f_cdf570fa8dd6_h

#include "flatland/canvas/svg_canvas.h"
#include "flatland/integrator/integrator.h"
#include "flatland/math/refraction.h"

FLATLAND_BEGIN_NAMESPACE

// from pbrt v3 with modifications
template<typename ScalarType>
Vector2<ScalarType> reflect(const Vector2<ScalarType> &wo, const Normal2<ScalarType> &normal) {
    return -wo + 2 * wo.dot(normal) * normal;
}

// This integrator follows a camera ray and only expects at each surface interaction a mirror reflection
template<typename ScalarType>
class PathMirrorReflection2 : public IntegratorType<2, ScalarType> {
public:
    using Base = IntegratorType<2, ScalarType>;
    using Color = ColorType<3, ScalarType>;
    using Scalar = ScalarType;

    PathMirrorReflection2(const PropertySet &ps) : IntegratorType<2, ScalarType>(ps) {
        max_depth = ps.getProperty("max_depth", 5);
    }

    virtual ~PathMirrorReflection2() {
    }

    Color trace(
            const Scene2<ScalarType> *scene,
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
            Vector2<ScalarType> reflectedDirection = reflect<ScalarType>(wi, me.n.normalized());

            Ray2f reflectedRay(me.p + reflectedDirection * ScalarType{0.01}, reflectedDirection, ScalarType{0.0}, ScalarType{20000.0});

            trace(scene, reflectedRay, depth+1);
        }
        else {
            ray.max_t = 100.0f;
        }

        Base::canvas_->add(ray);

        return Color{Scalar{0.0}, Scalar{0.0}, Scalar{0.0}};
    }

private:
    int max_depth = 0;
};

using PathMirrorReflection2f = PathMirrorReflection2<float>;
using PathMirrorReflection2d = PathMirrorReflection2<double>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_PathMirrorReflection_747251b4_9a7b_4321_a54f_cdf570fa8dd6_h
