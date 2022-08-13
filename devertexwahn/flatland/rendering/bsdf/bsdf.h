/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Rendering_Material_11d72dcc_a416_4530_b706_b1e69711c5c5_h
#define Flatland_Rendering_Material_11d72dcc_a416_4530_b706_b1e69711c5c5_h

#include "flatland/core/object.h"
#include "flatland/rendering/property_set.h"
#include "flatland/core/reference_counted.h"
#include "flatland/imaging/color.h"

FLATLAND_BEGIN_NAMESPACE

enum class InterfaceInteraction {
    SpecularTransmission,
    MirrorReflection
};

class BSDF : public Object {
public:
    BSDF(const PropertySet &ps);

    float refraction_index() const;

    InterfaceInteraction interface_interaction_type() const;

    Color3f color() {
        return color_;
    }

private:
    Color3f color_{1.f, 1.f, 1.f};
    float refraction_index_ = 0.f;
    InterfaceInteraction interface_interaction_ = InterfaceInteraction::SpecularTransmission;
};

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Rendering_Material_11d72dcc_a416_4530_b706_b1e69711c5c5_h
