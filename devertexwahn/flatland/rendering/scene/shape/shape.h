/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_Shape_1aef9b7b_dc9e_4d86_96c6_3552a8001293_h
#define De_Vertexwahn_Flatland_Shape_1aef9b7b_dc9e_4d86_96c6_3552a8001293_h

#include "flatland/rendering/bsdf/svg_material.h"
#include "flatland/rendering/scene/shape/emitter.h"
#include "flatland/rendering/scene/shape/medium_event.h"

#include "math/axis_aligned_bounding_box.h"
#include "math/frame.h"

#include "core/object.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template <typename ScalarType, unsigned int Dimension>
class ShapeTypeBase : public Object {
public:
    ShapeTypeBase() : transform_(identity<ScalarType>()) {}

    ShapeTypeBase(const Transform44f &transform) : transform_(transform) {
    };

    ShapeTypeBase(const PropertySet &ps) : transform_(ps.get_property<Transform44f>("transform")) {

    }

    virtual ~ShapeTypeBase() {}

    [[nodiscard]]
    virtual AxisAlignedBoundingBoxType<ScalarType, Dimension> bounds() const = 0; // minimal AABB of shape

    virtual bool intersect(const RayType<ScalarType, Dimension> &ray, MediumEventType<ScalarType, Dimension> &me) const = 0;

    const Transform44Type<ScalarType>& transform() const {
        return transform_;
    };

    void set_bsdf(ReferenceCounted<BSDFType<ScalarType, Dimension>> bxdf) {
        bsdf_ = bxdf;
    }
    ReferenceCounted<BSDFType<ScalarType, Dimension>> bsdf() const {
        return bsdf_;
    }

    void set_emitter(ReferenceCounted<Emitter<ScalarType, Dimension>> emitter) {
        emitter_ = emitter;
    }
    ReferenceCounted<Emitter<ScalarType, Dimension>> emitter() const {
        return emitter_;
    }
    virtual bool is_emitter() const { return emitter_ != nullptr; }

protected:
    Transform44Type<ScalarType> transform_; // transform from object space to world space
    ReferenceCounted<BSDFType<ScalarType, Dimension>> bsdf_{nullptr};
    ReferenceCounted<Emitter<ScalarType, Dimension>> emitter_{nullptr};
};

template <typename ScalarType, unsigned int Dimension>
class ShapeType : public ShapeTypeBase<ScalarType, Dimension> {
public:
    ShapeType() {}
	ShapeType(const Transform44f& transform)  : ShapeTypeBase<ScalarType, Dimension>(transform) {
    };
    ShapeType(const PropertySet& ps) : ShapeTypeBase<ScalarType, Dimension>(ps) {

    }

	virtual ~ShapeType() {};
};

template <typename ScalarType>
class ShapeType<ScalarType, 2> : public ShapeTypeBase<ScalarType, 2> {
public:
    ShapeType() {}
    ShapeType(const Transform44f& transform)  : ShapeTypeBase<ScalarType, 2>(transform) {
    };
    ShapeType(const PropertySet& ps) : ShapeTypeBase<ScalarType, 2>(ps) {

    }

    virtual std::string convert_to_svg(const int svgCanvasWidth, const int svgCanvasHeight) const = 0;

    [[nodiscard]]
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

protected:
};

template <typename ScalarType>
using Shape2 = ShapeType<ScalarType, 2>;
template <typename ScalarType>
using Shape3 = ShapeType<ScalarType, 3>;

using Shape2f = Shape2<float>;
using Shape2d = Shape2<double>;
using Shape3f = Shape3<float>;
using Shape3d = Shape3<double>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Shape_1aef9b7b_dc9e_4d86_96c6_3552a8001293_h
