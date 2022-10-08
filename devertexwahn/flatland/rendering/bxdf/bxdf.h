/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_Rendering_Material_11d72dcc_a416_4530_b706_b1e69711c5c5_h
#define De_Vertexwahn_Flatland_Rendering_Material_11d72dcc_a416_4530_b706_b1e69711c5c5_h

#include "core/object.h"
#include "core/reference_counted.h"
#include "flatland/rendering/property_set.h"
#include "imaging/color.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

enum class InterfaceInteraction {
    SpecularTransmission,
    MirrorReflection
};

class BxDF : public Object {
public:
    BxDF(const PropertySet &ps);

    float refraction_index() const;

    InterfaceInteraction interface_interaction_type() const;

    [[nodiscard]] virtual Color3f evaluate(const Vector3f &wo, const Vector3f &wi) const = 0;
    virtual Vector3f sample(const Vector3f &wo, Vector3f& out_wi) const = 0;

private:
    //Color3f color_{1.f, 1.f, 1.f};
    float refraction_index_ = 0.f;
    InterfaceInteraction interface_interaction_ = InterfaceInteraction::SpecularTransmission;
};

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Rendering_Material_11d72dcc_a416_4530_b706_b1e69711c5c5_h
