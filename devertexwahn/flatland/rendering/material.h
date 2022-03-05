/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Rendering_Material_11d72dcc_a416_4530_b706_b1e69711c5c5_h
#define Flatland_Rendering_Material_11d72dcc_a416_4530_b706_b1e69711c5c5_h

#include "flatland/core/namespace.h"
#include "flatland/rendering/property_set.h"
#include "flatland/core/reference_counted.h"
#include "flatland/imaging/color.h"

FLATLAND_BEGIN_NAMESPACE

enum class eInterfaceInteraction {
    specularTransmission,
    mirrorReflection
};

class Material {
public:
    Material();

    Material(const PropertySet& ps);

    float refraction_index() const;

    eInterfaceInteraction interface_interaction_type() const;

private:
	float refraction_index_ = 0.f;
    eInterfaceInteraction interface_interaction_  = eInterfaceInteraction::specularTransmission;
};

class SvgMaterial : public Material {
public:
    SvgMaterial() {

    }

    SvgMaterial(const PropertySet& ps) : Material(ps) {
        stroke_color_ = ps.get_property<Color3f>("stroke_color", Color3f(0.f, 0.f, 0.f));
        stroke_width_ = ps.get_property<float>("stroke_width", 1.f);
        fill_color_ = ps.get_property<Color3f>("fill_color", Color3f(1.f, 1.f, 1.f));
    }

    // svg output related
    void set_stroke_color(Color3f color);
    Color3f stroke_color() const;
    void set_stroke_width(const float width);
    float stroke_width() const;
    void set_fill_color(Color3f color);
    Color3f fill_color() const;


private:
    Color3f stroke_color_ = Color3f(0.f, 0.f, 0.f);
    Color3f fill_color_ = Color3f(1.f, 1.f, 1.f);
    float stroke_width_ = 1.f;
};

ReferenceCounted<SvgMaterial> create_material(const PropertySet& ps);

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Rendering_Material_11d72dcc_a416_4530_b706_b1e69711c5c5_h
