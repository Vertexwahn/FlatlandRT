/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Shape_1aef9b7b_dc9e_4d86_96c6_3552a8001293_h
#define Flatland_Shape_1aef9b7b_dc9e_4d86_96c6_3552a8001293_h

#include "flatland/math/util.h"
#include "flatland/core/material.h"
#include "flatland/core/non_copyable.h"
#include "flatland/core/reference_counted.h"
#include "flatland/math/transform.h"
#include "flatland/core/namespace.h"


FLATLAND_BEGIN_NAMESPACE

template <unsigned int Dimension, typename ScalarType>
struct Intersection {
	PointType<Dimension, ScalarType> p;   // intersection point
	ScalarType t;                     // distance to intersection point
	NormalType<Dimension, ScalarType> n;  // normal at intersection point
};

template <typename ScalarType>
using Intersection2 = Intersection<2, ScalarType>;

using Intersection2f = Intersection2<float>;

template <unsigned int Dimension, typename ScalarType>
struct MediumEventType : public Intersection<Dimension, ScalarType> {
    Material *material = nullptr;
};

template <typename ScalarValue>
using MediumEvent2 = MediumEventType<2, ScalarValue>;
template <typename ScalarValue>
using MediumEvent3 = MediumEventType<3, ScalarValue>;

using MediumEvent2f = MediumEvent2<float>;
using MediumEvent2d = MediumEvent2<double>;
using MediumEvent3f = MediumEvent3<float>;
using MediumEvent3d = MediumEvent3<double>;

template <unsigned int Dimension, typename ScalarType>
class ShapeType : NonCopyable {
public:
    ShapeType() : transform_(identity<ScalarType>()) {}
	ShapeType(const Transform44f& transform)  : transform_(transform) {

    };
	virtual ~ShapeType() {};

	virtual bool intersect(const RayType<Dimension, ScalarType> &ray, MediumEventType<Dimension, ScalarType> &me) const = 0;

	virtual std::string convertToSvg(const int svgCanvasWidth, const int svgCanvasHeight) const = 0;

	void setMaterial(ReferenceCounted<Material> material) {
        material_ = material;
	}
	ReferenceCounted<Material> getMaterial() const {
        return material_;
	}

	const Transform44Type<ScalarType>& getTransform() const {
        return transform_;
	};

    std::string convertColor3fToSVGColor3b(const Color3f &color) const {
        std::stringstream ss;
        ss << color.red() * 255.0f << "," << color.green() * 255.0f << "," << color.blue() * 255.0f;
        return ss.str();
    }

    std::string convertMaterialToSvgStyle(const Material *material) const {
        std::stringstream ss;

        ss << "style=\"";
        ss << "fill:rgb(" << convertColor3fToSVGColor3b(material->getFillColor()) << ");";
        ss << "stroke-width:" << material->getStrokeWidth() << ";";
        ss << "stroke:rgb(" << convertColor3fToSVGColor3b(material->getStrokeColor()) << ")";
        ss << "\"";

        return ss.str();
    }
protected:
    Transform44Type<ScalarType> transform_;
	ReferenceCounted<Material> material_{nullptr};
};

template <typename ScalarType>
using Shape2 = ShapeType<2, ScalarType>;

using Shape2f = Shape2<float>;
using Shape2d = Shape2<double>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Shape_1aef9b7b_dc9e_4d86_96c6_3552a8001293_h