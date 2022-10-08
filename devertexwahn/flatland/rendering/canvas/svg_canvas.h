/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_Rendering_SvgCanvas_40c12871_00a2_441f_842c_9fb53ad77721_h
#define De_Vertexwahn_Flatland_Rendering_SvgCanvas_40c12871_00a2_441f_842c_9fb53ad77721_h

#include "math/axis_aligned_bounding_box.h"
#include "math/ray.h"
#include "flatland/rendering/canvas/label.h"
#include "flatland/rendering/scene/shape/shape.h"

#include <cstdint>
#include <fstream>

DE_VERTEXWAHN_BEGIN_NAMESPACE

template <typename ScalarType>
class SvgCanvasType {
public:
    using Ray = Ray2<ScalarType>;
    using Label = LabelType2<ScalarType>;
    using Point = PointType<2, ScalarType>;
    using Scalar = ScalarType;
    using Shape = Shape2<ScalarType>;
    using AxisAlignedBoundingBox = AxisAlignedBoundingBox2<ScalarType>;

    SvgCanvasType(const int width, const int height) : width_(width), height_(height) {
    }

    SvgCanvasType(const Vector2i size) : SvgCanvasType(size.x(), size.y()) {
    }

    int width() const {
        return width_;
    }

    int height() const {
        return height_;
    }

    Vector2i size() const {
        return Vector2i(width_, height_);
    }

    void add(const Ray &ray) {
        rays_.push_back(ray);
    }

    std::vector<Ray> rays() const {
        return rays_;
    }

    size_t ray_count() const {
        return rays_.size();
    }

	void add(const Shape* shape) {
        shapes_.push_back(shape);
    }

    size_t shape_count() const {
        return shapes_.size();
    }

    void add(const Label &text) {
        texts_.push_back(text);
    }

    void add(const AxisAlignedBoundingBox& aabb) {
        bounds_.push_back(aabb);
    }

    std::string to_string() const {
        std::stringstream out;

        out << build_header();

        for(const auto *shape : shapes_) {
            out << shape->convert_to_svg(width_, height_);
        }

        for (const auto &ray : rays_) {
            out << convert_to_svg_string(ray);
        }

        for (const auto &text : texts_) {
            out << convert_to_svg_string(text);
        }

        for (const auto &bound : bounds_) {
            out << convert_to_svg_string(bound);
        }

        out << build_footer();

        return out.str();
    }

	void store(std::string_view filename) const {
        std::ofstream svg(filename.data());

        if (!svg.is_open()) {
            throw std::runtime_error("File could not be opened");
        }

        svg << to_string().c_str();
        svg.close();
    }

private:
	std::string convert_to_svg_string(const Ray &ray) const {
        Point start = ray(ray.min_t);
        Point end = ray(ray.max_t - Scalar{19.5});
        Point end2 = ray(ray.max_t);

        std::stringstream ss;
        ss << "    ";
        ss << "    ";

        // do not use marker end for short rays
        if(ray.max_t - ray.min_t < Scalar{20.0}) {
            ss << "<line x1=\"" << start.x() << "\" y1=\"" << height_ - start.y() << "\" x2=\"" << end2.x() << "\" y2=\""
               << height_ - end2.y() << "\" stroke=\"rgb(242, 101, 34)\" stroke-width=\"3\" />\n";
        }
        else {
            ss << "<line x1=\"" << start.x() << "\" y1=\"" << height_ - start.y() << "\" x2=\"" << end.x() << "\" y2=\""
               << height_ - end.y() << "\" stroke=\"rgb(242, 101, 34)\" stroke-width=\"3\" marker-end=\"url(#arrow_head)\"/>\n";
        }

        return ss.str();
    }

	std::string convert_to_svg_string(const Label &text) const {
        std::stringstream ss;
        ss << "    ";
        ss << "    ";
        ss << "<text x=\"" << text.position.x() << "\" "
           << "y=\""
           << height_ - text.position.y() << "\""
           << " " << "font-size=\"2em\"" << ">"
           << text.text.c_str()
           << "</text>"
           << "\n";
        return ss.str();
    }

    std::string convert_to_svg_string(const AxisAlignedBoundingBox &aabb) const {
        std::stringstream ss;
        ss << "    ";
        ss << "    ";
        ss << "<rect x=\"" << aabb.min_.x() << "\" "
           << "y=\"" << height_ - aabb.min_.y() - aabb.height() << "\" "
           << "width=\"" << aabb.width() << "\" "
           << "height=\"" << aabb.height() << "\" "
           << "style=\"fill:none;stroke:#008000\" "
           << "/>"
           << "\n";
        return ss.str();
    }

	std::string build_header() const {
        std::stringstream ss;

        ss << R"___(<?xml version="1.0" encoding="UTF-8"?>)___" << "\n";
        ss << R"___(<svg xmlns="http://www.w3.org/2000/svg")___" << "\n"
           << R"___(    xmlns:xlink="http://www.w3.org/1999/xlink" width=")___"
           << width_ << "\" "
           << "height=\"" << height_ << "\" "
           << R"___(baseProfile="full" version="1.1" viewBox="0 0 )___" << width_ << " " << height_ << "\">";
           ss << R"___(
    <defs>
        <marker id="arrow_head" markerHeight="8" markerWidth="8" orient="auto" refX="0.125" refY="3" viewBox="0 0 5 6">
            <path fill="rgb(242, 101, 34)" d="M0,0 V6 L5,3 Z"/>
        </marker>
    </defs>
    <rect width="100%" height="100%" fill="white"/>
    <g transform="scale(1, 1)">)___" << "\n";

        return ss.str();
    }

    std::string build_footer() const {
        std::stringstream footer;
        footer << "    </g>" << "\n";
        footer << "</svg>";
        return footer.str();
    }

protected:
    std::vector<AxisAlignedBoundingBox> bounds_;
    std::vector<const Shape*> shapes_;
    std::vector<Label> texts_;
    std::vector<Ray> rays_;

    int width_;
    int height_;
};

using SvgCanvas2f = SvgCanvasType<float>;
using SvgCanvas2d = SvgCanvasType<double>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Rendering_SvgCanvas_40c12871_00a2_441f_842c_9fb53ad77721_h