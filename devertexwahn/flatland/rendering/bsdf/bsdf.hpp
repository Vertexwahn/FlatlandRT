/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_Rendering_BSDF_11d72dcc_a416_4530_b706_b1e69711c5c5_h
#define De_Vertexwahn_Flatland_Rendering_BSDF_11d72dcc_a416_4530_b706_b1e69711c5c5_h

#include "flatland/rendering/property_set.h"
#include "flatland/rendering/shape/medium_event.hpp"

#include "imaging/color.hpp"

#include "core/object.hpp"
#include "core/reference_counted.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template <typename ScalarType, unsigned int Dimension>
struct BSDFSampleType {
    using Scalar = ScalarType;
    using Vector = VectorType<ScalarType, Dimension>;
    using MediumEvent = MediumEventType<ScalarType, Dimension>;

    Vector wi;
    Vector wo;
    const MediumEvent* me;
};

using BSDFSample2f = BSDFSampleType<float, 2>;
using BSDFSample3f = BSDFSampleType<float, 3>;

template <typename ScalarType>
using BSDFSample2 = BSDFSampleType<ScalarType, 2>;
template <typename ScalarType>
using BSDFSample3 = BSDFSampleType<ScalarType, 3>;

// This interface expects that wi and wo are a normalized vectors on the unit sphere
// that are given in the reflectance coordinate system.
// The BSDF is not aware of any world space coordinate system.
// If you want to talk to this interface you have first to transform from world space to
// reflection space
template <typename ScalarType, unsigned int Dimension>
class BSDFType : public Object {
public:
    using Scalar = ScalarType;
    using Point2 = PointType<ScalarType, 2>;
    using Vector = VectorType<ScalarType, Dimension>;
    using Color = ColorTypeRGB<ScalarType, 3>;
    using BSDFSample = BSDFSampleType<ScalarType, Dimension>;
    using MediumEvent = MediumEventType<ScalarType, Dimension>;

    BSDFType(const PropertySet &ps) {}

    [[nodiscard]]
    virtual Color sample(BSDFSample& sample, const Point2& sample_point) const = 0;
    [[nodiscard]]
    virtual Scalar pdf(const BSDFSample& sample) const = 0;
    [[nodiscard]]
    virtual Color evaluate(const BSDFSample& sample) const = 0;
};

using BSDF2f = BSDFType<float, 2>;
using BSDF2d = BSDFType<double, 2>;
using BSDF3f = BSDFType<float, 3>;
using BSDF3d = BSDFType<double, 3>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Rendering_BSDF_11d72dcc_a416_4530_b706_b1e69711c5c5_h
