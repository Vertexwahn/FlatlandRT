/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Disk2f_9deef3ad_50b0_4b94_b279_21c462b565e7_h
#define Flatland_Disk2f_9deef3ad_50b0_4b94_b279_21c462b565e7_h

#include "flatland/core/namespace.h"
#include "flatland/core/property_set.h"
#include "flatland/math/intersection.h"
#include "flatland/scene/shape/shape.h"

FLATLAND_BEGIN_NAMESPACE

template <typename ScalarType>
class Disk2 : public Shape2<ScalarType> {
public:
    using Point = Point2<ScalarType>;
    using Vector = Vector2<ScalarType>;
    using Normal = Normal2<ScalarType>;
    using Frame = Frame2<ScalarType>;
    using Scalar = ScalarType;

    Disk2(const PropertySet& ps) : Shape2<Scalar>(ps) {
        auto radius = ps.getProperty<float>("radius");
        radius_ = radius;
    }

    bool intersect(const Ray2<Scalar> &ray, MediumEvent2<Scalar> &me) const override {
        Point intersectionPoint;
        Normal normal;
        float distance;

        Point center{Scalar{0.0},Scalar{0.0}};
        center = Shape2<Scalar>::transform_ * center;
        bool bHit = intersectRayCircle(ray.origin,
                                       ray.direction,
                                       center,
                                       radius_,
                                       intersectionPoint,
                                       distance,
                                       normal);

        me.t = distance;
        me.p = intersectionPoint;
        me.n = normal;
        me.material = Shape2<ScalarType>::material_.get();
        me.frame = Frame{normal, Vector{normal.y(), -normal.x()}};

        return bHit;
    }

    std::string convertToSvg(const int svgCanvasWidth, const int svgCanvasHeight) const override {
        std::stringstream ss;

        Point center{Scalar{0.0}, Scalar{0.0}};
        center = Shape2<Scalar>::getTransform() * center;

        ss << "<circle cx=\"" << center.x() << "\" cy=\"" << svgCanvasHeight - center.y() << "\" r=\""
           << getRadius()
           << "\"";

        ReferenceCounted<Material> material = Shape2<Scalar>::getMaterial();
        if (material) {
            ss << " ";
            ss << Shape2<Scalar>::convertMaterialToSvgStyle(material.get());
        }

        ss << ">";
        ss << "</circle>";
        ss << "\n";
        return ss.str();
    }

    Scalar getRadius() const {
        return radius_;
    }

private:
    Scalar radius_;
};

using Disk2f = Disk2<float>;
using Disk2d = Disk2<double>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Disk2f_9deef3ad_50b0_4b94_b279_21c462b565e7_h