/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Polygon2f_53d2e474_422c_4d90_a16d_ace0175300ea_h
#define Flatland_Polygon2f_53d2e474_422c_4d90_a16d_ace0175300ea_h

#include "flatland/core/namespace.h"
#include "flatland/math/geometry.h"
#include "flatland/math/intersection.h"
#include "flatland/math/transform.h"
#include "flatland/math/util.h"
#include "flatland/scene/shape/shape.h"

#include <iomanip>

FLATLAND_BEGIN_NAMESPACE

template <typename ScalarType>
class Polygon2 : public Shape2<ScalarType> {
public:
	Polygon2(const Transform44Type<ScalarType>& transform, const Point2<ScalarType> *points, const size_t pointCount)  : Shape2<ScalarType>(
            transform) {
        std::copy(&points[0], &points[pointCount], std::back_inserter(points_));

        if(!isCounterClockwiseOrder<ScalarType>(points_))
            throw std::runtime_error("Points not counter clock wise");
    }

	bool intersect(const Ray2<ScalarType> &ray_, MediumEvent2<ScalarType> &me) const override {
        // transform ray into object space
        auto toObjectSpace = Shape2<ScalarType>::transform_.inverse();

        std::cout << ray_ << std::endl;
        Ray2<ScalarType> ray  = toObjectSpace * ray_;

        bool hit = false;

        auto pointCount = points_.size();

        for (size_t i = 0; i < pointCount; ++i) {
            float t;
            Point2<ScalarType> intersectionPoint;
            Normal2<ScalarType> n;

            bool result = intersectRayLineSegment(
                    ray.origin,
                    ray.direction,
                    points_[i],
                    points_[(i + 1) % pointCount],
                    intersectionPoint, t, n);

            if (result) {
                if (!hit) {
                    hit = true;
                    me.p = intersectionPoint;
                    me.t = t;
                    me.n = n;
                } else {
                    if (t < me.t) {
                        me.p = intersectionPoint;
                        me.t = t;
                        me.n = n;
                    }
                }
            }
        }

        if(hit) {
            me.p = Shape2<ScalarType>::transform_ * me.p;
            me.n = Shape2<ScalarType>::transform_ * me.n;
            me.n.normalize();
            me.material = Shape2<ScalarType>::material_.get();
        }

        return hit;
    }

    virtual std::string convertToSvg(const int svgCanvasWidth, const int svgCanvasHeight) const override  {
        std::stringstream ss;
        ss << "<path ";

        ReferenceCounted<Material> material = Shape2<ScalarType>::getMaterial();
        if (material) {
            ss << " ";
            ss << Shape2<ScalarType>::convertMaterialToSvgStyle(material.get());
        }

        ss << std::endl;
        ss << " d=\"M ";

        for (const auto &point : points_) {
            auto pt = Shape2<ScalarType>::transform_ * point;
            ss << std::setprecision(10) << pt.x();
            ss << " ";
            ss << std::setprecision(10) << svgCanvasHeight + -1.0f * pt.y();
            ss << " ";
        }

        ss << " Z\" />" << std::endl;
        return ss.str();
    }

private:
    std::vector<Point2<ScalarType>> points_;
};

using Polygon2f = Polygon2<float>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Polygon2f_53d2e474_422c_4d90_a16d_ace0175300ea_h