/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/material.h"

FLATLAND_BEGIN_NAMESPACE

Material::Material() {

}

Material::Material(const PropertySet &ps) {
    refraction_index_ = ps.get_property<float>("refraction_index", 0.f);

    //bool mirrorReflection_ = false; -> enum class eInterfaceInteraction {specularTransmission, mirrorReflection}
    // <string name="interface_interaction" value= "specular_transmission">

    std::string interfaceInteraction = ps.get_property<std::string>("interface_interaction",
                                                                    "specular_transmission");

    if (interfaceInteraction == "specular_transmission") {
        interface_interaction_ = eInterfaceInteraction::specularTransmission;
    } else if (interfaceInteraction == "mirror_reflection") {
        interface_interaction_ = eInterfaceInteraction::mirrorReflection;
    } else {
        interface_interaction_ = eInterfaceInteraction::specularTransmission;
    }
}

void SvgMaterial::set_stroke_color(Color3f color) {
    stroke_color_ = color;
}

void SvgMaterial::set_stroke_width(const float width) {
    stroke_width_ = width;
}

void SvgMaterial::set_fill_color(Color3f color) {
    fill_color_ = color;
}

Color3f SvgMaterial::stroke_color() const {
    return stroke_color_;
}

Color3f SvgMaterial::fill_color() const {
    return fill_color_;
}

float SvgMaterial::stroke_width() const {
    return stroke_width_;
}

float Material::refraction_index() const {
    return refraction_index_;
}

eInterfaceInteraction Material::interface_interaction_type() const {
    return interface_interaction_;
}

ReferenceCounted<SvgMaterial>
create_material(const PropertySet &ps) {
    return make_reference_counted<SvgMaterial>(ps);
}

FLATLAND_END_NAMESPACE

