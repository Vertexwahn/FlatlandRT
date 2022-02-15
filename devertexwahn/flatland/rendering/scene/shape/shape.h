/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Okapi_Shape_1aef9b7b_dc9e_4d86_96c6_3552a8001293_h
#define Okapi_Shape_1aef9b7b_dc9e_4d86_96c6_3552a8001293_h

#include "flatland/core/object.h"
#include "flatland/math/axis_aligned_bounding_box.h"
#include "flatland/math/frame.h"
#include "flatland/rendering/material.h"

FLATLAND_BEGIN_NAMESPACE

template <unsigned int Dimension, typename ScalarType>
struct MediumEventType {
    using Scalar = ScalarType;
    using Point = PointType<Dimension, ScalarType>;
    using Normal = NormalType<Dimension, ScalarType>;
    using Frame = FrameType<Dimension, ScalarType>;

    Point p; // intersection point
    Scalar t; // distance to intersection point
    Frame geo_frame; // geo_frame regarding to world space
	Frame sh_frame; // shading frame

    Material *material = nullptr;
};

template <typename ScalarType>
using MediumEvent2 = MediumEventType<2, ScalarType>;
template <typename ScalarType>
using MediumEvent3 = MediumEventType<3, ScalarType>;

using MediumEvent2f = MediumEvent2<float>;
using MediumEvent2d = MediumEvent2<double>;
using MediumEvent3f = MediumEvent3<float>;
using MediumEvent3d = MediumEvent3<double>;

template <unsigned int Dimension, typename ScalarType>
class ShapeTypeBase : public Object {
public:
    ShapeTypeBase() : transform_(identity<ScalarType>()) {}

    ShapeTypeBase(const Transform44f& transform) : transform_(transform) {
    };

    ShapeTypeBase(const PropertySet& ps) : transform_(ps.get_property<Transform44f>("transform")) {

    }

    virtual ~ShapeTypeBase() {}

    [[nodiscard]]
    virtual AxisAlignedBoundingBoxType<Dimension, ScalarType> bounds() const = 0; // minimal AABB of shape

    virtual bool intersect(const RayType<Dimension, ScalarType> &ray, MediumEventType<Dimension, ScalarType> &me) const = 0;

    const Transform44Type<ScalarType>& transform() const {
        return transform_;
    };

protected:
    Transform44Type<ScalarType> transform_; // transform from object space to world space
};

template <unsigned int Dimension, typename ScalarType>
class ShapeType : public ShapeTypeBase<Dimension,ScalarType> {
public:
    ShapeType() {}
	ShapeType(const Transform44f& transform)  : ShapeTypeBase<Dimension,ScalarType>(transform) {
    };
    ShapeType(const PropertySet& ps) : ShapeTypeBase<Dimension,ScalarType>(ps) {

    }

	virtual ~ShapeType() {};

	void set_material(ReferenceCounted<Material> material) {
        material_ = material;
	}
	ReferenceCounted<Material> material() const {
        return material_;
	}

protected:
	ReferenceCounted<Material> material_{nullptr};
};

template <typename ScalarType>
class ShapeType<2, ScalarType> : public ShapeTypeBase<2,ScalarType> {
public:
    ShapeType() {}
    ShapeType(const Transform44f& transform)  : ShapeTypeBase<2,ScalarType>(transform) {
    };
    ShapeType(const PropertySet& ps) : ShapeTypeBase<2,ScalarType>(ps) {

    }

    virtual std::string convert_to_svg(const int svgCanvasWidth, const int svgCanvasHeight) const = 0;

    std::string convert_color3f_to_svg_color3b(const Color3f &color) const {
        std::stringstream ss;
        ss << color.red() * 255.f << "," << color.green() * 255.f << "," << color.blue() * 255.f;
        return ss.str();
    }

    std::string convert_material_to_svg_style(const SvgMaterial *material) const {
        std::stringstream ss;

        ss << "style=\"";
        ss << "fill:rgb(" << convert_color3f_to_svg_color3b(material->fill_color()) << ");";
        ss << "stroke-width:" << material->stroke_width() << ";";
        ss << "stroke:rgb(" << convert_color3f_to_svg_color3b(material->stroke_color()) << ")";
        ss << "\"";

        return ss.str();
    }

    void set_material(ReferenceCounted<SvgMaterial> material) {
        material_ = material;
    }
    ReferenceCounted<SvgMaterial> material() const {
        return material_;
    }

protected:

    ReferenceCounted<SvgMaterial> material_{nullptr};
};

template <typename ScalarType>
using Shape2 = ShapeType<2, ScalarType>;
template <typename ScalarType>
using Shape3 = ShapeType<3, ScalarType>;

using Shape2f = Shape2<float>;
using Shape2d = Shape2<double>;
using Shape3f = Shape3<float>;
using Shape3d = Shape3<double>;

FLATLAND_END_NAMESPACE

#endif // end define Okapi_Shape_1aef9b7b_dc9e_4d86_96c6_3552a8001293_h