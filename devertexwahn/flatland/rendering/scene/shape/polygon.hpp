/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_Polygon2f_53d2e474_422c_4d90_a16d_ace0175300ea_h
#define De_Vertexwahn_Flatland_Polygon2f_53d2e474_422c_4d90_a16d_ace0175300ea_h

#include "math/geometry.hpp"
#include "math/intersection.hpp"
#include "flatland/rendering/scene/shape/shape.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template<typename ScalarType>
class Polygon2 : public Shape2<ScalarType> {
public:
    using Point = Point2<ScalarType>;
    using Vector = Vector2<ScalarType>;
    using Normal = Normal2<ScalarType>;
    using Frame = Frame2<ScalarType>;
    using Scalar = ScalarType;

    Polygon2(const Transform44Type<ScalarType> &transform, const Point *points, const size_t point_count)
            : Shape2<ScalarType>(
            transform) {
        std::copy(&points[0], &points[point_count], std::back_inserter(points_));

        ensure_valid_points();
    }

    Polygon2(const PropertySet &ps) : Shape2<Scalar>(ps) {
        std::string filename = ps.get_property<std::string>("filename");

        std::stringstream ss;
        ss << ps.get_property<std::string>("parent_path") << "/" << filename;

        points_ = load_2D_ply<float>(ss.str());

        ensure_valid_points();
    }

    [[nodiscard]]
    virtual AxisAlignedBoundingBox2f bounds() const override {
        throw std::runtime_error("Not implemented yet");
    }

    bool intersect(const Ray2<ScalarType> &ray_, MediumEvent2<ScalarType> &me) const override {
        // transform ray into object space
        auto to_object_space = Shape2<ScalarType>::transform_.inverse();

        Ray2<ScalarType> ray = to_object_space * ray_;

        bool hit = false;

        auto point_count = points_.size();

        for (size_t i = 0; i < point_count; ++i) {
            float t;
            Point2<ScalarType> intersection_point;
            Normal2<ScalarType> n;

            bool result = intersect_ray_line_segment(
                    ray.origin,
                    ray.direction,
                    points_[i],
                    points_[(i + 1) % point_count],
                    intersection_point, t, n);

            if (result) {
                if (!hit) {
                    hit = true;
                    me.p = intersection_point;
                    me.t = t;
                    me.geo_frame = Frame{n, Vector{n.y(), -n.x()}};
                } else {
                    if (t < me.t) {
                        me.p = intersection_point;
                        me.t = t;
                        me.geo_frame = Frame{n, Vector{n.y(), -n.x()}};
                    }
                }
            }
        }

        if (hit) {
            me.p = Shape2<ScalarType>::transform_ * me.p;
            me.shape = this;
            me.geo_frame.n = Shape2<ScalarType>::transform_ * me.geo_frame.n;
            me.geo_frame.n.normalize();
            me.geo_frame.t = Shape2<ScalarType>::transform_ * me.geo_frame.t;
            me.geo_frame.t.normalize();
        }

        return hit;
    }

    virtual std::string convert_to_svg(const int svgCanvasWidth, const int svgCanvasHeight) const override {
        std::stringstream ss;
        ss << "    ";
        ss << "    ";
        ss << "<path";

        ReferenceCounted<SvgMaterial> material = std::static_pointer_cast<SvgMaterial>(Shape2<ScalarType>::bsdf());

        if (material) {
            ss << " ";
            ss << Shape2<ScalarType>::convert_material_to_svg_style(material.get());
        }

        ss << " d=\"M ";

        for (const auto &point: points_) {
            auto pt = Shape2<ScalarType>::transform_ * point;
            ss << std::setprecision(10) << pt.x();
            ss << " ";
            ss << std::setprecision(10) << svgCanvasHeight + -1.f * pt.y();
            ss << " ";
        }

        ss << " Z\" />" << "\n";
        return ss.str();
    }

protected:
    void ensure_valid_points() {
        for (const auto &point: points_) {
            if(point.has_nans()) {
                if (point.x()) {
                    throw std::runtime_error("x component of point is NaN");
                }
                if (std::isnan(point.y())) {
                    throw std::runtime_error("y component of point is NaN");
                }
            }
            if (!std::isfinite(point.x())) {
                throw std::runtime_error("x component of point is not finite");
            }
            if (!std::isfinite(point.y())) {
                throw std::runtime_error("y component of point is not finite");
            }
        }

        if (!is_counter_clockwise_order<ScalarType>(points_)) {
            throw std::runtime_error("Points not counter clock wise");
        }
    }

protected:
    std::vector<Point2<ScalarType>> points_;
};

using Polygon2f = Polygon2<float>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Polygon2f_53d2e474_422c_4d90_a16d_ace0175300ea_h
