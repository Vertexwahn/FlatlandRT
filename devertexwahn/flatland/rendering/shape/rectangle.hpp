/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_Rectangle2_28cd6d3d_9cf6_4418_a453_ca763c184461_h
#define De_Vertexwahn_Flatland_Rectangle2_28cd6d3d_9cf6_4418_a453_ca763c184461_h

#include "core/namespace.hpp"
#include "math/intersection.hpp"
#include "flatland/rendering/shape/shape.hpp"

#include <iomanip>

DE_VERTEXWAHN_BEGIN_NAMESPACE

template <typename ScalarType>
class Rectangle2 : public Shape2<ScalarType> {
public:
    using Point = Point2<ScalarType>;
    using Vector = Vector2<ScalarType>;
    using Normal = Normal2<ScalarType>;
    using Frame = Frame2<ScalarType>;
    using Scalar = ScalarType;

    Rectangle2(const PropertySet& ps) : Shape2<ScalarType>(ps) {
        width_ = ps.get_property<Scalar>("width");
        height_ = ps.get_property<Scalar>("height");

        assert(width_ > Scalar{0.0});
        assert(height_ > Scalar{0.0});

        Point minimum_{-width_/Scalar{2.0}, -height_/Scalar{2.0}};
        Point maximum_{width_/Scalar{2.0}, height_/Scalar{2.0}};

        Point points[4] = {minimum_,
                           minimum_ + Vector{width_, Scalar{0.0}},
                           maximum_,
                           minimum_ + Vector{Scalar{0.0}, height_}
        };

        for(int i = 0; i < 4; ++i) {
            points_[i] = Shape2<Scalar>::transform_ * points[i];
        }
    }

    [[nodiscard]]
    virtual AxisAlignedBoundingBox2f bounds() const override {
        throw std::runtime_error("Not implemented yet");
    }

	bool intersect(const Ray2<Scalar>& ray, MediumEvent2<Scalar>& its) const override {
		bool hit = false;

		// 0 -> 1
		// 1 -> 2
		// 2 -> 3
		// 3 -> 4
		for (int i = 0; i < 4; ++i) {
            float t;
            Point intersection_point;
            Normal n;

			bool result =
                    intersect_ray_line_segment(ray.origin, ray.direction, points_[i], points_[(i + 1) % 4],
                                               intersection_point, t, n);

			if (result) {
				if (!hit) {
					hit = true;
					its.p = intersection_point;
					its.t = t;
					its.geo_frame = Frame{n, Vector{n.y(), -n.x()}};
				} else {
					if (t < its.t) {
						its.p = intersection_point;
						its.t = t;
                        its.geo_frame = Frame{n, Vector{n.y(), -n.x()}};
					}
				}
			}
		}

		return hit;
	}

    std::string convert_to_svg(const int svgCanvasWidth, const int svgCanvasHeight) const override {
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

        for (const auto &point : points_) {
            auto pt = point;
            ss << std::setprecision(10) << pt.x();
            ss << " ";
            ss << std::setprecision(10) << svgCanvasHeight + -1.f * pt.y();
            ss << " ";
        }

        ss << " Z\" />" << "\n";
        return ss.str();
	};

private:
    /*
     * D               C
     * +---------------+
     * |               |
     * |               |
     * |               |
     * +---------------+
     * A               B
     */
    Point points_[4];
    Scalar width_;
    Scalar height_;
};

using Rectangle2f = Rectangle2<float>;
using Rectangle2d = Rectangle2<double>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Rectangle2_28cd6d3d_9cf6_4418_a453_ca763c184461_h
