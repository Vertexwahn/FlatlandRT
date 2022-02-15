/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Okapi_Path_408ff181_0029_4c38_bb7d_5031a65b6f8d_h
#define Okapi_Path_408ff181_0029_4c38_bb7d_5031a65b6f8d_h

#include "flatland/rendering/canvas/svg_canvas.h"
#include "flatland/rendering/integrator/integrator.h"

FLATLAND_BEGIN_NAMESPACE

// This integrator follows a sensor ray and only expects at each surface interaction a specular transmission
class PathSpecularTransmission : public Integrator2f {
public:
    PathSpecularTransmission(const PropertySet& ps);
    virtual ~PathSpecularTransmission();

    Color3f trace(const Scene2f *scene,  Sampler* sampler, Ray2f &ray, const int depth) const override;

private:
    int max_depth = 0;
};

FLATLAND_END_NAMESPACE

#endif // end define Okapi_Path_408ff181_0029_4c38_bb7d_5031a65b6f8d_h
