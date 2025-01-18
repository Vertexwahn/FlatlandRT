/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_Rendering_Intersector_fa38b1e4_1fd0_4ac7_b5c0_116ac6d531c8_h
#define De_Vertexwahn_Flatland_Rendering_Intersector_fa38b1e4_1fd0_4ac7_b5c0_116ac6d531c8_h

#include "core/object.hpp"
#include "core/reference_counted.hpp"
#include "math/ray.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template<typename ScalarType, unsigned int Dimension>
struct MediumEventType;

template<typename ScalarType, unsigned int Dimension>
class ShapeType;

template<typename ScalarType, unsigned int Dimension>
class IntersectorType : public Object {
public:
    using Ray = RayType<ScalarType, Dimension>;
    using MediumEvent = MediumEventType<ScalarType, Dimension>;
    using Shape = ShapeType<ScalarType, Dimension>;

    virtual void build_acceleration_structure(std::vector<ReferenceCounted<Shape>> shapes) = 0;

    virtual bool intersect(const Ray &ray, MediumEvent &me) const = 0;
};

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Rendering_Intersector_fa38b1e4_1fd0_4ac7_b5c0_116ac6d531c8_h
