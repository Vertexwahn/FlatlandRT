/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
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

Color3f SvgMaterial::evaluate(const Vector2f &wo, const Vector2f &wi) const {
    return Color3f{0.f, 0.f, 0.f};
}

Vector2f SvgMaterial::sample(const Vector2f &wo, Vector2f &out_wi) const {
    return Vector2f{0.f, 0.f};
}

DE_VERTEXWAHN_END_NAMESPACE
