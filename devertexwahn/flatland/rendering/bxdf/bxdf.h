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

template <unsigned int Dimension, typename ScalarType>
class BxDF : public Object {
public:
    using Scalar = ScalarType;
    using Color = ColorType<3, ScalarType>;
    using Vector = VectorType<Dimension, ScalarType>;

    BxDF(const PropertySet &ps) {
        refraction_index_ = ps.get_property<float>("refraction_index", 1.f);

        std::string interface_interaction = ps.get_property<std::string>("interface_interaction",
                                                                         "specular_transmission");

        if (interface_interaction == "specular_transmission") {
            interface_interaction_ = InterfaceInteraction::SpecularTransmission;
        } else if (interface_interaction == "mirror_reflection") {
            interface_interaction_ = InterfaceInteraction::MirrorReflection;
        } else {
            interface_interaction_ = InterfaceInteraction::SpecularTransmission;
        }
    }

    Scalar refraction_index() const {
        return refraction_index_;
    }

    InterfaceInteraction interface_interaction_type() const {
        return interface_interaction_;
    }

    [[nodiscard]] virtual Color evaluate(const Vector &wo, const Vector &wi) const = 0;
    virtual Vector sample(const Vector &wo, Vector& wi) const = 0;

private:
    Scalar refraction_index_ = Scalar{0};
    InterfaceInteraction interface_interaction_ = InterfaceInteraction::SpecularTransmission;
};

using BxDF2f = BxDF<2, float>;
using BxDF2d = BxDF<2, double>;
using BxDF3f = BxDF<3, float>;
using BxDF3d = BxDF<3, double>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Rendering_Material_11d72dcc_a416_4530_b706_b1e69711c5c5_h
