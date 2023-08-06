/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_Rendering_BSDF_11d72dcc_a416_4530_b706_b1e69711c5c5_h
#define De_Vertexwahn_Flatland_Rendering_BSDF_11d72dcc_a416_4530_b706_b1e69711c5c5_h

#include "flatland/rendering/property_set.h"

#include "imaging/color.h"

#include "core/object.h"
#include "core/reference_counted.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

enum class InterfaceInteraction {
    SpecularTransmission,
    MirrorReflection
};

template <unsigned int Dimension, typename ScalarType>
struct BSDFSampleType {
    using Scalar = ScalarType;
    using Vector = VectorType<Dimension, ScalarType>;

    Vector wi;
    Vector wo;
};

using BSDFSample2f = BSDFSampleType<2, float>;
using BSDFSample3f = BSDFSampleType<3, float>;

template <typename ScalarType>
using BSDFSample2 = BSDFSampleType<2, ScalarType>;
template <typename ScalarType>
using BSDFSample3 = BSDFSampleType<3, ScalarType>;

// This interface expects that wi and wo are a normalized vectors on the unit sphere
// that are given in the reflectance coordinate system.
// The BSDF is not aware of any world space coordinate system.
// If you want to talk to this interface you have first to transform from world space to
// reflection space
template <unsigned int Dimension, typename ScalarType>
class BSDFType : public Object {
public:
    using Scalar = ScalarType;
    using Point2 = PointType<2, ScalarType>;
    using Vector = VectorType<Dimension, ScalarType>;
    using Color = ColorType<3, ScalarType>;
    using BSDFSample = BSDFSampleType<Dimension, ScalarType>;

    BSDFType(const PropertySet &ps) {
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

    // todo: move this to a dielectric material class
    Scalar refraction_index() const {
        return refraction_index_;
    }

    [[nodiscard]]
    InterfaceInteraction interface_interaction_type() const {
        return interface_interaction_;
    }

    [[nodiscard]] virtual Color sample(BSDFSample& sample, const Point2& sample_point) const = 0;
    [[nodiscard]] virtual Scalar pdf(const BSDFSample& sample) const = 0;
    [[nodiscard]] virtual Color evaluate(const BSDFSample& sample) const = 0;

private:
    Scalar refraction_index_ = Scalar{0};
    InterfaceInteraction interface_interaction_ = InterfaceInteraction::SpecularTransmission;
};

using BSDF2f = BSDFType<2, float>;
using BSDF2d = BSDFType<2, double>;
using BSDF3f = BSDFType<3, float>;
using BSDF3d = BSDFType<3, double>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Rendering_BSDF_11d72dcc_a416_4530_b706_b1e69711c5c5_h
