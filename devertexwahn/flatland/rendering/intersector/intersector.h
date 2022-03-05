/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Rendering_Intersector_fa38b1e4_1fd0_4ac7_b5c0_116ac6d531c8_h
#define Flatland_Rendering_Intersector_fa38b1e4_1fd0_4ac7_b5c0_116ac6d531c8_h

#include "flatland/core/object.h"
#include "flatland/core/reference_counted.h"
#include "flatland/math/ray.h"

FLATLAND_BEGIN_NAMESPACE

template<unsigned int Dimension, typename ScalarType>
struct MediumEventType;

template<unsigned int Dimension, typename ScalarType>
class ShapeType;

template<unsigned int Dimension, typename ScalarType>
class IntersectorType : public Object {
public:
    using Ray = RayType<Dimension, ScalarType>;
    using MediumEvent = MediumEventType<Dimension, ScalarType>;
    using Shape = ShapeType<Dimension, ScalarType>;

    virtual void build_acceleration_structure(std::vector<ReferenceCounted < Shape>>

    shapes) = 0;

    virtual bool intersect(const Ray &ray, MediumEvent &me) const = 0;
};

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Rendering_Intersector_fa38b1e4_1fd0_4ac7_b5c0_116ac6d531c8_h
