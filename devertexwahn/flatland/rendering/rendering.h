/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_Rendering_render_9fbae968_34ec_4c10_942b_a94e982cd3e8_h
#define De_Vertexwahn_Flatland_Rendering_render_9fbae968_34ec_4c10_942b_a94e982cd3e8_h

#include "flatland/rendering/integrator/integrator.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

void render(Integrator2f *integrator, ReferenceCounted<SvgCanvas2f> canvas_, const Scene2f *scene);

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Rendering_render_9fbae968_34ec_4c10_942b_a94e982cd3e8_h
