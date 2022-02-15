/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Rendering_render_9fbae968_34ec_4c10_942b_a94e982cd3e8_h
#define Flatland_Rendering_render_9fbae968_34ec_4c10_942b_a94e982cd3e8_h

#include "flatland/rendering/integrator/integrator.h"

FLATLAND_BEGIN_NAMESPACE

void render(Integrator2f* integrator, ReferenceCounted<SvgCanvas2f> canvas_, const Scene2f* scene);

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Rendering_render_9fbae968_34ec_4c10_942b_a94e982cd3e8_h
