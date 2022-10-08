/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/bxdf/svg_material.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

Color3f SvgMaterial::stroke_color() const {
    return stroke_color_;
}

Color3f SvgMaterial::fill_color() const {
    return fill_color_;
}

float SvgMaterial::stroke_width() const {
    return stroke_width_;
}

DE_VERTEXWAHN_END_NAMESPACE
