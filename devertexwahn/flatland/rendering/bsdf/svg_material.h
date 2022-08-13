/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Core_SvgMaterial_975da5be_8f82_4c9d_9029_3a06f8529a3a_h
#define Flatland_Core_SvgMaterial_975da5be_8f82_4c9d_9029_3a06f8529a3a_h

#include "flatland/rendering/bsdf/bsdf.h"

FLATLAND_BEGIN_NAMESPACE

class SvgMaterial final : public BSDF {
public:
    SvgMaterial(const PropertySet &ps) : BSDF(ps) {
        stroke_color_ = ps.get_property<Color3f>("stroke_color", Color3f{0.f, 0.f, 0.f});
        stroke_width_ = ps.get_property<float>("stroke_width", 1.f);
        fill_color_ = ps.get_property<Color3f>("fill_color", Color3f{1.f, 1.f, 1.f});
    }

    Color3f stroke_color() const;

    float stroke_width() const;

    Color3f fill_color() const;

private:
    Color3f stroke_color_ = Color3f{0.f, 0.f, 0.f};
    Color3f fill_color_ = Color3f{1.f, 1.f, 1.f};
    float stroke_width_ = 1.f;
};

FLATLAND_END_NAMESPACE

#endif // end define Okapi_SvgMaterial_975da5be_8f82_4c9d_9029_3a06f8529a3a_h
