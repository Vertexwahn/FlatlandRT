/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/integrator/path_specular_transmission.h"

#include "flatland/core/logging.h"
#include "flatland/scene/scene.h"
#include "flatland/math/refraction.h"

FLATLAND_BEGIN_NAMESPACE

PathSpecularTransmission::PathSpecularTransmission(const PropertySet& ps) : Integrator2f(ps) {
}

PathSpecularTransmission::~PathSpecularTransmission() {
}

Color3f PathSpecularTransmission::trace(
        const Scene2f *scene,
        Ray2f &ray,
        const int depth) const {
    if(depth > max_depth)
        return Color3f{0.0f, 0.0f, 0.0f}; //ColorConstants3f::Black;

    MediumEvent2f me;
    bool hit = scene->intersect(ray, me);

    if(hit) {
        ray.max_t = me.t;

        // compute refraction
        Vector2f refractedDirection;
        Vector2f wi = -ray.direction;
        wi.normalize();

        bool valid = false;

        // inside or outside?
        if (me.n.normalized().dot(wi) < 0) {
            valid = refract(wi, -me.n.normalized(), 1.6f / 1.0f, refractedDirection);
        } else {
            valid = refract(wi, me.n.normalized(), 1.0f / 1.6f, refractedDirection);
        }

        std::cout << "Valid refraction: " << valid << std::endl;
        assert(valid);
        if(!valid)
            return Color3f{0.0f, 0.0f, 0.0f};

        refractedDirection.normalize();

        Ray2f refractedRay(me.p + refractedDirection * 0.01f, refractedDirection, 0.0f, 20000.0f);

        trace(scene, refractedRay, depth+1);
    }
    else {
        ray.max_t = 100.0f;
    }

    canvas_->add(ray);

    return Color3f{0.0f, 0.0f, 0.0f};
}

FLATLAND_END_NAMESPACE