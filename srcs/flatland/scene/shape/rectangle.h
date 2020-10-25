/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Rectangle2_28cd6d3d_9cf6_4418_a453_ca763c184461_h
#define Flatland_Rectangle2_28cd6d3d_9cf6_4418_a453_ca763c184461_h

#include "flatland/core/namespace.h"
#include "flatland/math/intersection.h"
#include "flatland/scene/shape/shape.h"

#include <iomanip>

FLATLAND_BEGIN_NAMESPACE

template <typename ScalarType>
class Rectangle2 : public Shape2<ScalarType> {
public:
    using Point = Point2<ScalarType>;
    using Vector = Vector2<ScalarType>;
    using Normal = Normal2<ScalarType>;
    using Frame = Frame2<ScalarType>;
    using Scalar = ScalarType;

    Rectangle2(const PropertySet& ps) : Shape2<ScalarType>(ps) {
        width_ = ps.getProperty<Scalar>("width");
        height_ = ps.getProperty<Scalar>("height");

        assert(width_ > Scalar{0.0});
        assert(height_ > Scalar{0.0});

        Point minimum_{-width_/Scalar{2.0}, -height_/Scalar{2.0}};
        Point maximum_{width_/Scalar{2.0}, height_/Scalar{2.0}};

        Point points[4] = {minimum_,
                           minimum_ + Vector{width(), Scalar{0.0}},
                           maximum_,
                           minimum_ + Vector{Scalar{0.0}, height()}
        };

        for(int i = 0; i < 4; ++i) {
            points_[i] = Shape2<Scalar>::transform_ * points[i];
        }
    }

	Scalar width() const {
		return width_;
	}

	Scalar height() const {
		return height_;
	}

	bool intersect(const Ray2<Scalar>& ray, MediumEvent2<Scalar>& its) const override {
		bool hit = false;

		// 0 -> 1
		// 1 -> 2
		// 2 -> 3
		// 3 -> 4
		for (int i = 0; i < 4; ++i) {
            float t;
            Point intersectionPoint;
            Normal n;

			bool result =
			  intersectRayLineSegment(ray.origin, ray.direction, points_[i], points_[(i + 1) % 4], intersectionPoint, t, n);

			if (result) {
				if (!hit) {
					hit = true;
					its.p = intersectionPoint;
					its.t = t;
					its.n = n;
					its.frame = Frame{n, Vector{n.y(), -n.x()}};
				} else {
					if (t < its.t) {
						its.p = intersectionPoint;
						its.t = t;
                        its.n = n;
                        its.frame = Frame{n, Vector{n.y(), -n.x()}};
					}
				}
			}
		}

		return hit;
	}

    std::string convertToSvg(const int svgCanvasWidth, const int svgCanvasHeight) const override {
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
            auto pt = point;
            ss << std::setprecision(10) << pt.x();
            ss << " ";
            ss << std::setprecision(10) << svgCanvasHeight + -1.0f * pt.y();
            ss << " ";
        }

        ss << " Z\" />" << std::endl;
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

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Rectangle2_28cd6d3d_9cf6_4418_a453_ca763c184461_h