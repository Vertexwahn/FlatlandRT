/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Core_QuadtreeIntersector_f0b7e663_c015_4d14_bb05_707399d47537_h
#define Flatland_Core_QuadtreeIntersector_f0b7e663_c015_4d14_bb05_707399d47537_h

#include "flatland/math/axis_aligned_bounding_box.h"
#include "flatland/math/util.h"
#include "flatland/rendering/intersector/intersector.h"
#include "flatland/rendering/scene/shape/polygon.h"
#include "flatland/rendering/scene/shape/triangle_mesh.h"

FLATLAND_BEGIN_NAMESPACE

using ShapeId = unsigned int;
using SubShapeId = unsigned int;

struct SubShapeIdentifier {
    ShapeId parent_shape_index;
    SubShapeId sub_shape_index;
};

template <unsigned int Dimension, typename ScalarType>
class IShapesSubBoundsProvider {
public:
    using AxisAlignedBoundingBox = AxisAlignedBoundingBoxType<Dimension, ScalarType>;

    [[nodiscard]]
    virtual std::vector<SubShapeIdentifier> sub_shape_ids() const = 0;

    [[nodiscard]]
    virtual AxisAlignedBoundingBox bounds() const = 0; // Minimal AABB of all shapes

    [[nodiscard]]
    virtual AxisAlignedBoundingBox bounds(const SubShapeIdentifier& id) const = 0; // Minimal AABB of a specific sub shape
};

using IShapesSubBoundsProvider2f = IShapesSubBoundsProvider<2, float>;

class TriangleMeshesBoundsProvider2f : public IShapesSubBoundsProvider2f {
public:
    explicit TriangleMeshesBoundsProvider2f(std::vector<ReferenceCounted<Shape2f>> shapes) : shapes_(shapes) {

    }

    virtual ~TriangleMeshesBoundsProvider2f() {}

    [[nodiscard]]
    std::vector<SubShapeIdentifier> sub_shape_ids() const override {
        std::vector<SubShapeIdentifier> sub_shape_ids;

        for(ShapeId shape_index = 0; shape_index < shapes_.size(); ++shape_index) {
            ReferenceCounted<TriangleMesh2f> tm = std::dynamic_pointer_cast<TriangleMesh2f>(shapes_[shape_index]);

            for(SubShapeId sub_shape_index = 0; sub_shape_index < tm->index_count() / 3; ++sub_shape_index) {
                SubShapeIdentifier identifier{shape_index, sub_shape_index};
                sub_shape_ids.push_back(identifier);
            }
        }

        return sub_shape_ids;
    }

    [[nodiscard]]
    virtual AxisAlignedBoundingBox2f bounds() const override {
        if (shapes_.size() == 0u) {
            return AxisAlignedBoundingBox2f {{0.f, 0.f}, {0.f, 0.f}};
        }

        AxisAlignedBoundingBox2f bb = shapes_[0]->bounds();
        for(size_t i = 1; i < shapes_.size(); ++i) {
            bb = union_aabb(bb, shapes_[i]->bounds());
        }

        return bb;
    }

    [[nodiscard]]
    AxisAlignedBoundingBox2f bounds(const SubShapeIdentifier& id) const override {
        ReferenceCounted<TriangleMesh2f> tm = std::dynamic_pointer_cast<TriangleMesh2f>(shapes_[id.parent_shape_index]);

        auto triangle_index  = id.sub_shape_index;

        size_t index0 = tm->indices()[triangle_index * 3u + 0u];
        size_t index1 = tm->indices()[triangle_index * 3u + 1u];
        size_t index2 = tm->indices()[triangle_index * 3u + 2u];

        auto transform = tm->transform();

        auto v0 = transform * tm->positions()[index0];
        auto v1 = transform * tm->positions()[index1];
        auto v2 = transform * tm->positions()[index2];

        auto current_min_x = min(v0.x(), v1.x(), v2.x());
        auto current_min_y = min(v0.y(), v1.y(), v2.y());

        auto current_max_x = max(v0.x(), v1.x(), v2.x());
        auto current_max_y = max(v0.y(), v1.y(), v2.y());

        return AxisAlignedBoundingBox2f{{current_min_x, current_min_y}, {current_max_x, current_max_y}};
    }

private:
    std::vector<ReferenceCounted<Shape2f>> shapes_;
};

struct QuadtreeNode {
    std::vector<SubShapeIdentifier> sub_shape_ids;
    AxisAlignedBoundingBox2f bounds;

    std::array<QuadtreeNode*, 4> nodes = {nullptr, nullptr, nullptr, nullptr}; // only nullptr for leave nodes

    [[nodiscard]]
    bool is_parent() const {
        assert((sub_shape_ids.empty() == false && nodes[0] == nullptr && nodes[1] == nullptr && nodes[2] == nullptr && nodes[3] == nullptr) ^
               (sub_shape_ids.empty() == true && (nodes[0] != nullptr || nodes[1] != nullptr || nodes[2] != nullptr || nodes[3] != nullptr)));
        return sub_shape_ids.empty();
    }

    [[nodiscard]]
    bool is_leave() const {
        return !is_parent();
    }
};

std::vector<AxisAlignedBoundingBox2f> split_bound(const AxisAlignedBoundingBox2f& bound);

struct QuadtreeBuildDescription {
    const IShapesSubBoundsProvider2f *tip;
    const AxisAlignedBoundingBox2f &bound;
    const std::vector<SubShapeIdentifier>& sub_shapes_ids;
    size_t maximum_desired_leave_size;
};

QuadtreeNode *build_quadtree(const QuadtreeBuildDescription& qtbd);

class QuadtreeIntersector : public IntersectorType<2, float> {
public:
    explicit QuadtreeIntersector(const PropertySet& ps) {}

    virtual ~QuadtreeIntersector() = default;

    // better name: squarify? cubeanize
    static AxisAlignedBoundingBox2f square_box(const AxisAlignedBoundingBox2f& bounds) {
        float size = std::max(bounds.width(), bounds.height());
        AxisAlignedBoundingBox2f squared_box;
        squared_box.min_ = bounds.min_;
        squared_box.max_ = bounds.min_ + Vector2f{size, size};
        return squared_box;
    }

    void build_acceleration_structure(std::vector<ReferenceCounted<Shape>> shapes) override {
        //assert(!shapes.empty());
        shapes_ = shapes;

        auto shape_bounds_provider = std::make_shared<TriangleMeshesBoundsProvider2f>(shapes);

        const size_t MAXIMUM_DESIRED_LEAVE_SIZE = 1;

        auto sub_shape_ids = shape_bounds_provider->sub_shape_ids();

        QuadtreeBuildDescription qtbd{
            shape_bounds_provider.get(),
            square_box(shape_bounds_provider->bounds()),
            sub_shape_ids,
            MAXIMUM_DESIRED_LEAVE_SIZE
        };

        root_ = build_quadtree(qtbd);
    }

    bool intersect(QuadtreeNode* node, const Ray &ray, MediumEvent &me) const {
        if (node == nullptr) {
            return false;
        }

        if(node->is_leave()) {
            if(node->bounds.intersect(ray.origin, ray.direction, ray.max_t, nullptr)) {
                //me.t = std::numeric_limits<float>::infinity();

                // visit each sub shape and find the closest intersection
                for (size_t i = 0; i < node->sub_shape_ids.size(); ++i) {
                    auto shape_index = node->sub_shape_ids[i].parent_shape_index;
                    ReferenceCounted<TriangleMesh2f> mesh = std::dynamic_pointer_cast<TriangleMesh2f>(shapes_[shape_index]);
                    const auto& V = mesh->positions();
                    const auto& position_indices = mesh->indices();

                    uint32_t f = node->sub_shape_ids[i].sub_shape_index;

                    Point2f p0 = V[position_indices[f * 3 + 0]];
                    Point2f p1 = V[position_indices[f * 3 + 1]];
                    Point2f p2 = V[position_indices[f * 3 + 2]];

                    // make use of polygon class to compute the intersection
                    std::array<Point2f,3> points_ = {p0, p1, p2};
                    Polygon2f poly{mesh->transform(), &points_[0], 3};

                    MediumEvent shape_me;
                    bool shape_hit = poly.intersect(ray, shape_me);

                    if(shape_hit && shape_me.t < me.t) {
                        me = shape_me;
                    }
                }

                return me.t != std::numeric_limits<float>::infinity();
            }
            else {
                return false;
            }
        }

        // node is parent
        assert(node->is_parent());
        bool child_hit = false;
        if(node->bounds.intersect(ray.origin, ray.direction, ray.max_t, nullptr)) {
            for(int i = 0; i < 4; ++i) {
                if(intersect(node->nodes[i], ray, me)) {
                    child_hit = true;
                }
            }
        }

        return child_hit;
    }

    bool intersect(const Ray &ray, MediumEvent &me) const override {
        me.t = std::numeric_limits<float>::infinity();
        return intersect(root_, ray, me);
    };

    [[nodiscard]]
    std::string to_string() const override {
        return "QuadtreeIntersector";
    }

    [[nodiscard]]
    QuadtreeNode* root_node() {
        return root_;
    }

private:
    std::vector<ReferenceCounted<Shape>> shapes_;
    QuadtreeNode* root_ = nullptr;
};

using QuadtreeIntersector2f = QuadtreeIntersector;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Core_QuadtreeIntersector_f0b7e663_c015_4d14_bb05_707399d47537_h
