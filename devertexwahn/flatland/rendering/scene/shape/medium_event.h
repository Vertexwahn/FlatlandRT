/*
 *  SPDX-FileCopyrightText: Copyright 2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_MediumEvent_0348b3b2_3d75_4489_a2a5_7d4a6f7c0801_h
#define De_Vertexwahn_Core_MediumEvent_0348b3b2_3d75_4489_a2a5_7d4a6f7c0801_h

#include "math/frame.h"

#include "core/namespace.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template <typename ScalarType, unsigned int Dimension>
class ShapeType;

template<typename ScalarType, unsigned int Dimension>
struct MediumEventType {
    using Scalar = ScalarType;
    using Point = PointType<ScalarType, Dimension>;
    using Normal = NormalType<ScalarType, Dimension>;
    using Frame = FrameType<ScalarType, Dimension>;

    Point p; // intersection point
    Scalar t; // distance to intersection point
    Frame geo_frame; // geo_frame regarding to world space
    Frame sh_frame; // shading frame
    Point2f uv; // UV surface coordinates

    const ShapeType<ScalarType, Dimension> *shape = nullptr;
};

template <typename ScalarType>
using MediumEvent2 = MediumEventType<ScalarType, 2>;
template <typename ScalarType>
using MediumEvent3 = MediumEventType<ScalarType, 3>;

using MediumEvent2f = MediumEvent2<float>;
using MediumEvent2d = MediumEvent2<double>;
using MediumEvent3f = MediumEvent3<float>;
using MediumEvent3d = MediumEvent3<double>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Core_MediumEvent_0348b3b2_3d75_4489_a2a5_7d4a6f7c0801_h
