/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/intersector/quadtree_intersector.h"

#include "core/logging.h"
#include "math/util.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

std::vector<AxisAlignedBoundingBox2f> split_bound(const AxisAlignedBoundingBox2f &bound) {
    // G---------H---------I
    // |         |         |
    // |   (2)   |   (3)   |
    // |         |         |
    // D---------E---------F
    // |         |         |
    // |   (0)   |   (1)   |
    // |         |         |
    // A---------B---------C

    auto mid_point_x = mid_point(bound.min_.x(), bound.max_.x());
    auto mid_point_y = mid_point(bound.min_.y(), bound.max_.y());

    Point2f A{bound.min_};
    Point2f B{mid_point_x, bound.min_.y()};
    Point2f D{bound.min_.x(), mid_point_y};
    Point2f E{mid_point_x, mid_point_y};
    Point2f F{bound.max_.x(), mid_point_y};
    Point2f H{mid_point_x, bound.max_.y()};
    Point2f I{bound.max_};

    std::vector<AxisAlignedBoundingBox2f> bounds;
    bounds.push_back(AxisAlignedBoundingBox2f{A, E});
    bounds.push_back(AxisAlignedBoundingBox2f{B, F});
    bounds.push_back(AxisAlignedBoundingBox2f{D, H});
    bounds.push_back(AxisAlignedBoundingBox2f{E, I});

    return bounds;
}

#define ASSERT_SUBTREE_BOUNDS_ARE_QUADS \
    assert(std::abs(subtree_bounds[0].width() - subtree_bounds[0].height()) < 0.001f); \
    assert(std::abs(subtree_bounds[1].width() - subtree_bounds[1].height()) < 0.001f); \
    assert(std::abs(subtree_bounds[2].width() - subtree_bounds[2].height()) < 0.001f); \
    assert(std::abs(subtree_bounds[3].width() - subtree_bounds[3].height()) < 0.001f);

#define ASSERT_SUB_SHAPE_COUNT \
    assert(qtbd.sub_shapes_ids.size() <= zones[0].size() + zones[1].size() + zones[2].size() + zones[3].size()); \
    assert(qtbd.sub_shapes_ids.size() >= zones[0].size()); \
    assert(qtbd.sub_shapes_ids.size() >= zones[1].size()); \
    assert(qtbd.sub_shapes_ids.size() >= zones[2].size()); \
    assert(qtbd.sub_shapes_ids.size() >= zones[3].size());

QuadtreeNode *build_quadtree(const QuadtreeBuildDescription &qtbd) {
    // no triangles, no cry
    if (qtbd.sub_shapes_ids.empty()) {
        return nullptr;
    }

    // if only a few triangles are left return a leave node
    if (qtbd.sub_shapes_ids.size() <= qtbd.maximum_desired_leave_size) {
        auto *leaf_node = new QuadtreeNode();
        leaf_node->sub_shape_ids = qtbd.sub_shapes_ids;
        leaf_node->bounds = qtbd.bound;
        return leaf_node;
    }

    auto subtree_bounds = split_bound(qtbd.bound);

    ASSERT_SUBTREE_BOUNDS_ARE_QUADS

    std::array<std::vector<SubShapeIdentifier>, 4> zones;

    // instead of checking if a triangle overlaps the subtree AABB we only
    // check if the AABB of the triangle overlaps the AABB of the subtree
    for (auto sub_shape_id: qtbd.sub_shapes_ids) {
        auto triangle_bounds = qtbd.tip->bounds(sub_shape_id);

        for (int i = 0; i < 4; ++i) {
            if (overlaps(triangle_bounds, subtree_bounds[i])) {
                zones[i].push_back(sub_shape_id);
            }
        }
    }

    // there should not be more triangle after the split
    ASSERT_SUB_SHAPE_COUNT

    // create a inner node
    auto *parent = new QuadtreeNode();
    parent->bounds = qtbd.bound;

    switch (qtbd.strategy) {
        case QuadtreeBuildStrategy::StopSplitIfOneChildHasAsManySubShapesAsParent: {
                // Stop if splitting would lead to one child element that has as many triangles as the corresponding parent
                for (int i = 0; i < 4; ++i) {
                    if (zones[i].size() == qtbd.sub_shapes_ids.size()) {
                        parent->sub_shape_ids = qtbd.sub_shapes_ids;
                        return parent;
                    }
                }
            }
            break;
        case QuadtreeBuildStrategy::StopSplitIfAtLeastTwoChildsHaveAsManySubShapesAsParent: {
                int votes = 0;
                for (int i = 0; i < 4; ++i) {
                    if (zones[i].size() == qtbd.sub_shapes_ids.size()) {
                        votes++;
                    }
                }

                if(votes > 1) {
                    parent->sub_shape_ids = qtbd.sub_shapes_ids;
                    return parent;
                }
            }
            break;
        case QuadtreeBuildStrategy::StopAtMaxDepth: {
                const int MAX_DEPTH = 6;
                if(qtbd.depth >= MAX_DEPTH)
                    return parent;
            }
            break;
        default:
            throw std::runtime_error("Unknown strategy.");
    }

    for (int i = 0; i < 4; ++i) {
        QuadtreeBuildDescription qtbd_child{
                qtbd.tip,
                subtree_bounds[i],
                zones[i],
                qtbd.maximum_desired_leave_size,
                qtbd.depth+1,
                qtbd.strategy
        };

        parent->nodes[i] = build_quadtree(qtbd_child);
    }

    return parent;
}

DE_VERTEXWAHN_END_NAMESPACE
