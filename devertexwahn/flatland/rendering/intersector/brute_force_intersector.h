/*
 *  SPDX-FileCopyrightText: Copyright 2022-2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_Rendering_BruteForceIntersector_81859627_2b07_46f2_b5f4_026825b75239_h
#define De_Vertexwahn_Flatland_Rendering_BruteForceIntersector_81859627_2b07_46f2_b5f4_026825b75239_h

#include "core/logging.h"
#include "flatland/rendering/intersector/intersector.h"
#include "flatland/rendering/property_set.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template<typename ScalarType, unsigned int Dimension>
class BruteForceIntersector : public IntersectorType<ScalarType, Dimension> {
public:
    using Shape = ShapeType<ScalarType, Dimension>;
    using Ray = RayType<ScalarType, Dimension>;
    using MediumEvent = MediumEventType<ScalarType, Dimension>;
    using Scalar = ScalarType;

    BruteForceIntersector(const PropertySet &ps) {
        LOG_INFO_WITH_LOCATION("Using brute force intersector");
    }

    void build_acceleration_structure(std::vector<ReferenceCounted<Shape>> shapes) override {
        shapes_ = shapes;
    }

    bool intersect(const Ray &ray, MediumEvent &me) const override {
        me.t = std::numeric_limits<Scalar>::infinity();

        for (auto shape: shapes_) {
            MediumEvent shape_me;
            bool shape_hit = shape->intersect(ray, shape_me);

            if (shape_hit && shape_me.t < me.t) {
                me = shape_me;
            }
        }

        return me.t != std::numeric_limits<Scalar>::infinity();
    }

    [[nodiscard]]
    virtual std::string to_string() const override {
        return "BruteForceIntersector";
    };

private:
    std::vector<ReferenceCounted<Shape>> shapes_;
};

using BruteForceIntersector2f = BruteForceIntersector<float, 2>;
using BruteForceIntersector2d = BruteForceIntersector<double, 2>;

using BruteForceIntersector3f = BruteForceIntersector<float, 3>;
using BruteForceIntersector3d = BruteForceIntersector<double, 3>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Rendering_BruteForceIntersector_81859627_2b07_46f2_b5f4_026825b75239_h
