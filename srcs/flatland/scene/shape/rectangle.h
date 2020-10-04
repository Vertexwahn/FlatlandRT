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

FLATLAND_BEGIN_NAMESPACE

template <typename ScalarType>
class Rectangle2 : public Shape2<ScalarType> {
public:
    using PointType = Point2<ScalarType>;
    using VectorType = Vector2<ScalarType>;
    using NormalType = Normal2<ScalarType>;

 	Rectangle2(const Transform44Type<ScalarType>& transform, const ScalarType width, const ScalarType height) : Shape2<ScalarType>(transform) {
 	    assert(width > ScalarType{0.0});
 	    assert(height > ScalarType{0.0});

		width_ = width;
		height_ = height;
	}

	ScalarType width() const {
		return width_;
	}

	ScalarType height() const {
		return height_;
	}

	bool intersect(const Ray2<ScalarType>& ray, MediumEvent2<ScalarType>& its) const override {
		/*
		 * D               C
		 * +---------------+
		 * |               |
		 * |               |
		 * |               |
		 * +---------------+
		 * A               B
		 */

        PointType minimum_{-width_/ScalarType{2.0}, -height_/ScalarType{2.0}};
        PointType maximum_{width_/ScalarType{2.0}, height_/ScalarType{2.0}};
        minimum_ = Shape2<ScalarType>::transform_ * minimum_;
        maximum_ = Shape2<ScalarType>::transform_ * maximum_;

        PointType points[4] = {minimum_,
                               minimum_ + VectorType(width(), ScalarType{0.0}),
                               maximum_,
                               minimum_ + VectorType(ScalarType{0.0}, height())
        };

		bool hit = false;

		// 0 -> 1
		// 1 -> 2
		// 2 -> 3
		// 3 -> 4
		for (int i = 0; i < 4; ++i) {
            float t;
            PointType intersectionPoint;
            NormalType n;

			bool result =
			  intersectRayLineSegment(ray.origin, ray.direction, points[i], points[(i + 1) % 4], intersectionPoint, t, n);

			if (result) {
				if (!hit) {
					hit = true;
					its.p = intersectionPoint;
					its.t = t;
					its.n = n;
				} else {
					if (t < its.t) {
						its.p = intersectionPoint;
						its.t = t;
                        its.n = n;
					}
				}
			}
		}

		return hit;
	}

    std::string convertToSvg(const int svgCanvasWidth, const int svgCanvasHeight) const override {
        std::stringstream ss;
        ss << "<rect x=\"" << getMinimum().x() << "\""
           << " y=\"" << svgCanvasHeight - height() - getMinimum().y() << "\" "
           << "width=\"" << width() << "\" "
           << "height=\"" << height() << "\"";

        ReferenceCounted<Material> material = Shape2<ScalarType>::getMaterial();
        if (material) {
            ss << " ";
            ss << Shape2<ScalarType>::convertMaterialToSvgStyle(material.get());
        }

        ss << ">";

        ss << "</rect>";

        return ss.str();
	};

	PointType getMinimum() const {
        PointType minimum{-width_ / ScalarType{2.0}, -height_ / ScalarType{2.0}};
        minimum = Shape2<ScalarType>::transform_ * minimum;
		return minimum;
	}

    PointType getMaximum() const {
        PointType maximum{width_ / ScalarType{2.0}, height_ / ScalarType{2.0}};
        maximum = Shape2<ScalarType>::transform_ * maximum;
		return maximum;
	}

private:
    ScalarType width_;
    ScalarType height_;
};

using Rectangle2f = Rectangle2<float>;
using Rectangle2d = Rectangle2<double>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Rectangle2_28cd6d3d_9cf6_4418_a453_ca763c184461_h