/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Rendering_BruteForceIntersector_81859627_2b07_46f2_b5f4_026825b75239_h
#define Flatland_Rendering_BruteForceIntersector_81859627_2b07_46f2_b5f4_026825b75239_h

#include "flatland/core/logging.h"
#include "flatland/rendering/intersector/intersector.h"
#include "flatland/rendering/property_set.h"

FLATLAND_BEGIN_NAMESPACE

template<unsigned int Dimension, typename ScalarType>
class BruteForceIntersector : public IntersectorType<Dimension, ScalarType> {
public:
    using Shape = ShapeType<Dimension, ScalarType>;
    using Ray = RayType<Dimension, ScalarType>;
    using MediumEvent = MediumEventType<Dimension, ScalarType>;
    using Scalar = ScalarType;

    BruteForceIntersector(const PropertySet &ps) {
        LOG(INFO) << "Using brute force integrator";
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

using BruteForceIntersector2f = BruteForceIntersector<2, float>;
using BruteForceIntersector2d = BruteForceIntersector<2, double>;

using BruteForceIntersector3f = BruteForceIntersector<3, float>;
using BruteForceIntersector3d = BruteForceIntersector<3, double>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Rendering_BruteForceIntersector_81859627_2b07_46f2_b5f4_026825b75239_h
