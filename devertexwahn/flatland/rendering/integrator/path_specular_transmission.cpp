/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/integrator/path_specular_transmission.hpp"

#include "core/logging.hpp"
#include "math/refract.hpp"
#include "flatland/rendering/scene/scene.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

PathSpecularTransmission::PathSpecularTransmission(const PropertySet &ps) : Integrator2f(ps) {
    max_depth = ps.get_property("max_depth", 5);
}

PathSpecularTransmission::~PathSpecularTransmission() {
}

Color3f PathSpecularTransmission::trace(
        const Scene2f *scene,
        Sampler *sampler,
        Ray2f &ray,
        const int depth) const {
    if (depth > max_depth)
        return Color3f{0.f, 0.f, 0.f}; //ColorConstants3f::Black;

    MediumEvent2f me;
    bool hit = scene->intersect(ray, me);

    if (hit) {
        ray.max_t = me.t;

        // compute refraction
        Vector2f refracted_direction;
        Vector2f wi = -ray.direction;
        wi.normalize();

        bool valid = false;

        // inside or outside?
        if (me.geo_frame.n.normalized().dot(wi) < 0) {
            valid = refract(wi, -me.geo_frame.n.normalized(), 1.6f / 1.f, refracted_direction);
        } else {
            valid = refract(wi, me.geo_frame.n.normalized(), 1.f / 1.6f, refracted_direction);
        }

        std::cout << "Valid refraction: " << valid << std::endl;
        assert(valid);
        if (!valid)
            return Color3f{0.f, 0.f, 0.f};

        refracted_direction.normalize();

        Ray2f refractedRay(me.p + refracted_direction * 0.01f, refracted_direction, 0.f, 20000.f);

        trace(scene, sampler, refractedRay, depth + 1);
    } else {
        ray.max_t = 100.f;
    }

    canvas_->add(ray);

    return Color3f{0.f, 0.f, 0.f};
}

DE_VERTEXWAHN_END_NAMESPACE
