/*
 *  SPDX-FileCopyrightText: Copyright 2022-2026 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_SvgMaterial_975da5be_8f82_4c9d_9029_3a06f8529a3a_h
#define De_Vertexwahn_Core_SvgMaterial_975da5be_8f82_4c9d_9029_3a06f8529a3a_h

#include "flatland/rendering/bsdf/bsdf.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

class SvgMaterial : public BSDF2f {
public:
    SvgMaterial(const PropertySet &ps) : BSDF2f(ps) {
        stroke_color_ = ps.get_property<ColorRGB3f>("stroke_color", ColorRGB3f{0.f, 0.f, 0.f});
        stroke_width_ = ps.get_property<float>("stroke_width", 1.f);
        fill_color_ = ps.get_property<ColorRGB3f>("fill_color", ColorRGB3f{1.f, 1.f, 1.f});

        std::string interface_interaction = ps.get_property<std::string>("interface_interaction",
                                                                         "specular_transmission");
    }

    [[nodiscard]] ColorRGB3f stroke_color() const;
    [[nodiscard]] float stroke_width() const;
    [[nodiscard]] ColorRGB3f fill_color() const;

    [[nodiscard]] ColorRGB3f sample(BSDFSample2f& sample, const Point2& sample_point) const override;
    [[nodiscard]] float pdf(const BSDFSample2f& sample) const override { return 0.f; };
    [[nodiscard]] ColorRGB3f evaluate(const BSDFSample2f& sample) const override;

private:
    ColorRGB3f stroke_color_{0.f, 0.f, 0.f};
    ColorRGB3f fill_color_{1.f, 1.f, 1.f};
    float stroke_width_{1.f};
};

class GlassMaterial : public SvgMaterial {
public:
    GlassMaterial() = default;

    // todo: move this to a dielectric material class
    [[nodiscard]] Scalar refraction_index() const {
        return refraction_index_;
    }
private:
    Scalar refraction_index_ = Scalar{0};
};

class MirrorMaterial : public SvgMaterial {
public:
    MirrorMaterial() = default;
};

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define Okapi_SvgMaterial_975da5be_8f82_4c9d_9029_3a06f8529a3a_h
