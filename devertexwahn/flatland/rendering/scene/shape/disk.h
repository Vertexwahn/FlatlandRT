/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Okapi_Disk2f_9deef3ad_50b0_4b94_b279_21c462b565e7_h
#define Okapi_Disk2f_9deef3ad_50b0_4b94_b279_21c462b565e7_h

#include "flatland/math/intersection.h"
#include "flatland/rendering/property_set.h"
#include "flatland/rendering/scene/shape/shape.h"

FLATLAND_BEGIN_NAMESPACE

template <typename ScalarType>
class Disk2 : public Shape2<ScalarType> {
public:
    using Point = Point2<ScalarType>;
    using Vector = Vector2<ScalarType>;
    using Normal = Normal2<ScalarType>;
    using Frame = Frame2<ScalarType>;
    using Scalar = ScalarType;
	using Ray = Ray2<Scalar>;

    Disk2(const PropertySet& ps) : Shape2<Scalar>(ps) {
        auto radius = ps.get_property<float>("radius");
        radius_ = radius;
    }

    [[nodiscard]]
    virtual AxisAlignedBoundingBox2f bounds() const override {
        throw std::runtime_error("Not implemented yet");
    }

    bool intersect(const Ray &ray, MediumEvent2<Scalar> &me) const override {
        Point intersection_point;
        Normal normal;
        Scalar distance = Scalar{0.0};

        Point center{Scalar{0.0},Scalar{0.0}};
        center = Shape2<Scalar>::transform_ * center;
        bool bHit = intersect_ray_n_sphere(ray.origin,
                                           ray.direction,
                                           center,
                                           radius_,
                                           intersection_point,
                                           distance,
                                           normal);

        if(!bHit)
            return false;

        me.t = distance;
        me.p = intersection_point;
        me.material = Shape2<ScalarType>::material_.get();
        me.geo_frame = Frame{normal, Vector{normal.y(), -normal.x()}};

        return bHit;
    }

    std::string convert_to_svg(const int svgCanvasWidth, const int svgCanvasHeight) const override {
        std::stringstream ss;

        Point center{Scalar{0.0}, Scalar{0.0}};
        center = Shape2<Scalar>::transform() * center;

        ss << "<circle cx=\"" << center.x() << "\" cy=\"" << svgCanvasHeight - center.y() << "\" r=\""
           << radius()
           << "\"";

        ReferenceCounted<SvgMaterial> material = Shape2<Scalar>::material();
        if (material) {
            ss << " ";
            ss << Shape2<Scalar>::convert_material_to_svg_style(material.get());
        }

        ss << ">";
        ss << "</circle>";
        ss << "\n";
        return ss.str();
    }

    Scalar radius() const {
        return radius_;
    }

private:
    Scalar radius_;
};

using Disk2f = Disk2<float>;
using Disk2d = Disk2<double>;

FLATLAND_END_NAMESPACE

#endif // end define Okapi_Disk2f_9deef3ad_50b0_4b94_b279_21c462b565e7_h