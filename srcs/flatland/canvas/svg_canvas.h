/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_SvgCanvas_40c12871_00a2_441f_842c_9fb53ad77721_h
#define Flatland_SvgCanvas_40c12871_00a2_441f_842c_9fb53ad77721_h

#include "flatland/canvas/label.h"
#include "flatland/scene/shape/shape.h"

#include <cstdint>
#include <fstream>

FLATLAND_BEGIN_NAMESPACE

template <typename ScalarType>
class SvgCanvasType {
public:
    using Ray = Ray2<ScalarType>;
    using Shape = Shape2<ScalarType>;
    using Label = LabelType2<ScalarType>;
    using Point = PointType<2, ScalarType>;
    using Scalar = ScalarType;

    SvgCanvasType(const int width, const int height) : width_(width), height_(height) {
    }

    SvgCanvasType(const Vector2i size) : SvgCanvasType(size.x(), size.y()) {
    }

    int getWidth() const {
        return width_;
    }

    int getHeight() const {
        return height_;
    }

    Vector2i getSize() const {
        return Vector2i(width_, height_);
    }

    void add(const Ray &ray) {
        rays_.push_back(ray);
    }

    std::vector<Ray> getRays() const {
        return rays_;
    }

    size_t getRayCount() const {
        return rays_.size();
    }

	void add(const Shape* shape) {
        shapes_.push_back(shape);
    }

    size_t getShapeCount() const {
        return shapes_.size();
    }

    void add(const Label &text) {
        texts_.push_back(text);
    }

    std::string toString() const {
        std::stringstream out;

        out << buildHeader();

        for(const auto *shape : shapes_) {
            out << shape->convertToSvg(width_, height_);
        }

        for (const auto &ray : rays_) {
            out << convertToSvgString(ray);
        }

        for (const auto &text : texts_) {
            out << convertToSvgString(text);
        }

        out << buildFooter();

        return out.str();
    }

	void store(std::string_view filename) const {
        std::ofstream svg(filename.data());

        if (!svg.is_open()) {
            throw std::runtime_error("File could not be opened");
        }

        svg << toString().c_str();
        svg.close();
    }

private:
	std::string convertToSvgString(const Ray &ray) const {
        Point start = ray(ray.min_t);
        Point end = ray(ray.max_t - Scalar{19.5});
        Point end2 = ray(ray.max_t);

        std::stringstream ss;

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

	std::string convertToSvgString(const Label &text) const {
        std::stringstream ss;
        ss << "<text x=\"" << text.position.x() << "\" "
           << "y=\""
           << height_ - text.position.y() << "\""
           << " " << "font-size=\"2em\"" << ">"
           << text.text.c_str()
           << "</text>"
           << "\n";
        return ss.str();
    }

	std::string buildHeader() const {
        std::stringstream ss;

        ss << R"SEPERATOR(<?xml version="1.0" encoding="UTF-8"?>)SEPERATOR";
        ss << R"SEPERATOR(<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" width=")SEPERATOR"
           << width_ << "\" "
           << "height=\"" << height_ << "\" "
           << R"SEPERATOR(baseProfile="full" version="1.1" viewBox="0 0 )SEPERATOR" << width_ << " " << height_ << "\">";
           ss << R"SEPERATOR(
    <defs>
    <marker id="arrow_head" markerHeight="8" markerWidth="8" orient="auto" refX="0.125" refY="3" viewBox="0 0 5 6">
             <path fill="rgb(242, 101, 34)" d="M0,0 V6 L5,3 Z"/>
    </marker>
     </defs>
    <rect width="100%" height="100%" fill="white"/>
    <g transform="scale(1, 1)">)SEPERATOR";

        return ss.str();
    }

    std::string buildFooter() const {
        std::string footer = R"(</g>
      </svg>)";

        return footer.c_str();
    }

protected:
    std::vector<const Shape*> shapes_;
    std::vector<Label> texts_;
    std::vector<Ray> rays_;

    int width_;
    int height_;
};

using SvgCanvas2f = SvgCanvasType<float>;
using SvgCanvas2d = SvgCanvasType<double>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_SvgCanvas_40c12871_00a2_441f_842c_9fb53ad77721_h