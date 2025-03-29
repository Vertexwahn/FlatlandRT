/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/bsdf/svg_material.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

ColorRGB3f SvgMaterial::stroke_color() const {
    return stroke_color_;
}

ColorRGB3f SvgMaterial::fill_color() const {
    return fill_color_;
}

float SvgMaterial::stroke_width() const {
    return stroke_width_;
}

ColorRGB3f SvgMaterial::sample(BSDFSample2f& sample, const Point2& sample_point) const {
    return ColorRGB3f{0.f};
}

ColorRGB3f SvgMaterial::evaluate(const BSDFSample2f& sample) const {
    return ColorRGB3f{0.f, 0.f, 0.f};
}

DE_VERTEXWAHN_END_NAMESPACE
