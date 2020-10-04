/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "material.h"

FLATLAND_BEGIN_NAMESPACE

Material::Material() {

}

Material::Material(const PropertySet& ps) {
    strokeColor_ = ps.getProperty<Color3f>("stroke_color", Color3f(0.0f, 0.0f, 0.0f));
    strokeWidth_ = ps.getProperty<float>("stroke_width", 1.0f);
    fillColor_ = ps.getProperty<Color3f>("fill_color", Color3f(1.0f, 1.0f, 1.0f));
    refractionIndex_ = ps.getProperty<float>("refraction_index", 0.0f);

    //bool mirrorReflection_ = false; -> enum class eInterfaceInteraction {specularTransmission, mirrorReflection}
    // <string name="interface_interaction" value= "specular_transmission">

    std::string interfaceInteraction = ps.getProperty<std::string>("interface_interaction", "specular_transmission");

    if(interfaceInteraction == "specular_transmission") {
        interfaceInteraction_ = eInterfaceInteraction::specularTransmission;
    }
    else if(interfaceInteraction == "mirror_reflection") {
        interfaceInteraction_ = eInterfaceInteraction::mirrorReflection;
    }
    else {
        interfaceInteraction_ = eInterfaceInteraction::specularTransmission;
    }
}

void Material::setStrokeColor(Color3f color) {
    strokeColor_ = color;
}

void Material::setStrokeWidth(const float width) {
    strokeWidth_ = width;
}

void Material::setFillColor(Color3f color) {
    fillColor_ = color;
}

Color3f Material::getStrokeColor() const {
    return strokeColor_;
}

Color3f Material::getFillColor() const {
    return  fillColor_;
}

float Material::getStrokeWidth() const {
    return strokeWidth_;
}

float Material::getRefractionIndex() const {
    return refractionIndex_;
}

eInterfaceInteraction  Material::getInterInteractionType() const {
    return interfaceInteraction_;
}

ReferenceCounted<Material>
createMaterial(const PropertySet &ps) {
    return makeReferenceCounted<Material>(ps);
}

FLATLAND_END_NAMESPACE

