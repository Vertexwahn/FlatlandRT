/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "flatland/math/axis_aligned_bounding_box.h"
#include "flatland/math/geometry.h"
#include "flatland/rendering/scene/shape/shape.h"

FLATLAND_BEGIN_NAMESPACE

template<typename ScalarType>
class TriangleMesh2 : public Shape2<ScalarType> {
public:
    using Point = Point2<ScalarType>;
    //using Vector = Vector2<ScalarType>;
    //using Normal = Normal2<ScalarType>;
    //using Frame = Frame2<ScalarType>;
    using Scalar = ScalarType;

    TriangleMesh2(
            const PropertySet &ps,
            const std::vector<Point> &positions,
            const std::vector<std::uint32_t> &position_indices) :
            Shape2<Scalar>(ps),
            positions_(positions),
            indices_(position_indices) {
        ensure_valid_data();
    }

    TriangleMesh2(const PropertySet &ps) :
            Shape2<Scalar>(ps) {
        std::string filename = ps.get_property<std::string>("filename");

        std::stringstream ss;
        ss << ps.get_property<std::string>("parent_path") << "/" << filename;

        load_obj2d<Scalar>(ss.str(), positions_, indices_);

        ensure_valid_data();
    }
    
    std::string convert_to_svg(const int svgCanvasWidth, const int svgCanvasHeight) const override {
        auto triangle_count = indices_.size() / 3;

        std::stringstream ss;

        for (size_t i = 0; i < triangle_count; ++i) {
            ss << "    ";
            ss << "    ";
            ss << "<path";

            ReferenceCounted<SvgMaterial> material = Shape2<ScalarType>::material();
            if (material) {
                ss << " ";
                ss << Shape2<ScalarType>::convert_material_to_svg_style(material.get());
            }

            ss << " d=\"M ";

            int index0 = indices_[i * 3 + 0];
            int index1 = indices_[i * 3 + 1];
            int index2 = indices_[i * 3 + 2];

            std::vector<Point2f> points = { positions_[index0], positions_[index1], positions_[index2] };

            for (const auto &point : points) {
                auto pt = Shape2<ScalarType>::transform_ * point;
                ss << fmt::format("{:.10}", pt.x()); //  std::setprecision(10) << pt.x();
                ss << " ";
                ss << fmt::format("{:.10}",  svgCanvasHeight - 1.f * pt.y()); // std::setprecision(10) << svgCanvasHeight + -1.f * pt.y();
                ss << " ";
            }

            ss << " Z\" />" << "\n";

        }

        return ss.str();
    }

    bool intersect(const RayType<2, ScalarType> &ray, MediumEventType<2, ScalarType> &me) const override {
        //throw std::runtime_error("Triangle mesh intersection not implemented");
        //LOG(ERROR) << "Triangle mesh intersection not implemented";
        std::cout << "Triangle mesh intersection not implemented" << std::endl;
        return false;
    }

    size_t index_count() const {
        return indices_.size();
    }

    size_t position_count() const {
        return positions_.size();
    }

    const std::vector<Point>& positions() const {
        return positions_;
    }

    const std::vector<std::uint32_t>& indices() const {
        return indices_;
    }

    size_t triangle_count() const {
        return indices_.size() / 3;
    }

    [[nodiscard]]
    AxisAlignedBoundingBox2<Scalar> bounds() const override {
        Point current_min = positions_[0];
        Point current_max = positions_[0];

        for(auto point : positions_) {
            if(current_min.x() > point.x()) {
                current_min.x() = point.x();
            }
            if(current_min.y() > point.y()) {
                current_min.y() = point.y();
            }
            if(current_max.x() < point.x()) {
                current_max.x() = point.x();
            }
            if(current_max.y() < point.y()) {
                current_max.y() = point.y();
            }
        }

        auto transform = Shape2<ScalarType>::transform();

        return AxisAlignedBoundingBox2<Scalar>{transform * current_min, transform * current_max};
    }

private:
    void ensure_valid_data() {
        ensure_valid_points();
        ensure_valid_indices();
    }

    void ensure_valid_points() {
        // check positions
        for(const auto& point : positions_) {
            if(std::isnan(point.x())) {
                throw std::runtime_error("x component of point is NaN");
            }
            if(std::isnan(point.y())) {
                throw std::runtime_error("y component of point is NaN");
            }
            if(!std::isfinite(point.x())) {
                throw std::runtime_error("x component of point is not finite");
            }
            if(!std::isfinite(point.y())) {
                throw std::runtime_error("y component of point is not finite");
            }
        }
    }

    void ensure_valid_indices() {
        // check indices
        size_t index_count = positions_.size();
        for (auto position_index : indices_) {
            if (position_index >= index_count) {
                throw std::runtime_error("Invalid index provided. Index is out of valid range.");
            }
        }

        // check orientation
        auto triangle_count = indices_.size() / 3;

        for (size_t i = 0; i < triangle_count; ++i) {
            int index0 = indices_[i * 3 + 0];
            int index1 = indices_[i * 3 + 1];
            int index2 = indices_[i * 3 + 2];

            std::vector<Point2f> points = { positions_[index0], positions_[index1], positions_[index2] };

            if(!is_counter_clockwise_order<ScalarType>(points)) {
                throw std::runtime_error("Points not counter clock wise");
            }
        }

        // todo: make sure every point is referenced via an index
    }

private:
    std::vector<Point> positions_;
    std::vector<std::uint32_t> indices_;
};

using TriangleMesh2f = TriangleMesh2<float>;

FLATLAND_END_NAMESPACE