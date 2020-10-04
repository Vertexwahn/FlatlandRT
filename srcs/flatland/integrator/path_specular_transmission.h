/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Path_408ff181_0029_4c38_bb7d_5031a65b6f8d_h
#define Flatland_Path_408ff181_0029_4c38_bb7d_5031a65b6f8d_h

#include "flatland/canvas/svg_canvas.h"
#include "flatland/integrator/integrator.h"

FLATLAND_BEGIN_NAMESPACE

// This integrator follows a camera ray and only expects at each surface interaction a specular transmission
class PathSpecularTransmission : public Integrator2f {
public:
    PathSpecularTransmission(const PropertySet& ps);
    virtual ~PathSpecularTransmission();

    Color3f trace(const Scene2f *scene, Ray2f &ray, const int depth) const override;
};

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Path_408ff181_0029_4c38_bb7d_5031a65b6f8d_h
