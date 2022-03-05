/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/intersector/quadtree_intersector.h"

#include "gmock/gmock.h"

using namespace flatland;

TEST(TriangleMeshesBoundsProvider2f, bounds) {
    // Arrange
    auto transform = identity<float>();
    PropertySet tm_ps;
    tm_ps.add_property("parent_path", std::string("flatland/scenes/terrain"));
    tm_ps.add_property("transform", transform);
    tm_ps.add_property("filename", std::string("terrain.obj2d"));
    ReferenceCounted<TriangleMesh2f> tm = std::make_shared<TriangleMesh2f>(tm_ps);

    // Act
    std::vector<ReferenceCounted<Shape2f>> shapes{tm};
    TriangleMeshesBoundsProvider2f tma{shapes};

    // Assert
    EXPECT_THAT(tma.bounds(), (AxisAlignedBoundingBox2f{{0.f, 0.f},{500.f, 500.f}}));

    EXPECT_THAT(tma.bounds(SubShapeIdentifier{0,0}), (AxisAlignedBoundingBox2f{{0.f, 0.f},{50.f, 50.f}}));
    EXPECT_THAT(tma.bounds(SubShapeIdentifier{0,1}), (AxisAlignedBoundingBox2f{{0.f, 0.f},{50.f, 50.f}}));

    EXPECT_THAT(tma.bounds(SubShapeIdentifier{0,2}), (AxisAlignedBoundingBox2f{{50.f, 0.f},{100.f, 50.f}}));
    EXPECT_THAT(tma.bounds(SubShapeIdentifier{0,3}), (AxisAlignedBoundingBox2f{{50.f, 0.f},{100.f, 50.f}}));

    EXPECT_THAT(tma.bounds(SubShapeIdentifier{0,4}), (AxisAlignedBoundingBox2f{{100.f, 0.f},{150.f, 50.f}}));
    EXPECT_THAT(tma.bounds(SubShapeIdentifier{0,5}), (AxisAlignedBoundingBox2f{{100.f, 0.f},{150.f, 50.f}}));

    EXPECT_THAT(tma.bounds(SubShapeIdentifier{0,6}), (AxisAlignedBoundingBox2f{{150.f, 0.f},{200.f, 50.f}}));
    EXPECT_THAT(tma.bounds(SubShapeIdentifier{0,7}), (AxisAlignedBoundingBox2f{{150.f, 0.f},{200.f, 50.f}}));

    EXPECT_THAT(tma.bounds(SubShapeIdentifier{0,20}), (AxisAlignedBoundingBox2f{{0.f, 50.f},{50.f, 100.f}}));

    EXPECT_THAT(tma.sub_shape_ids()[0].sub_shape_index, 0);
    EXPECT_THAT(tma.sub_shape_ids()[1].sub_shape_index, 1);
    EXPECT_THAT(tma.sub_shape_ids()[2].sub_shape_index, 2);
}

TEST(TriangleMeshesBoundsProvider2f, GivenNoShape_WhenComputingBound_ThenExpectZeroAABB) {
    TriangleMeshesBoundsProvider2f tmbp{{}};
    EXPECT_THAT(tmbp.bounds(), (AxisAlignedBoundingBox2f{{0.f, 0.f}, {0.f, 0.f}}));
}

TEST(TriangleMeshesBoundsProvider2f, Given2Shapes_WhenComputingBound_ThenExpectUnionBox) {
    // Arrange
    std::vector<Point2f> points = {
            {0.f, 0.f},
            {1.f, 0.f},
            {1.f, 1.f}
    };
    std::vector<std::uint32_t> indices = {0, 1, 2};

    auto transform1 = identity<float>();
    PropertySet ps1;
    ps1.add_property("transform", transform1);
    ReferenceCounted<TriangleMesh2f> tm1 = make_reference_counted<TriangleMesh2f>(ps1, points, indices);

    auto transform2 = translate(1.f, 0.f, 0.f);
    PropertySet ps2;
    ps2.add_property("transform", transform2);
    ReferenceCounted<TriangleMesh2f> tm2 = make_reference_counted<TriangleMesh2f>(ps2, points, indices);

    TriangleMeshesBoundsProvider2f tmbp{{tm1, tm2}};

    EXPECT_THAT(tmbp.bounds(), (AxisAlignedBoundingBox2f{{0.f, 0.f}, {2.f, 1.f}}));
}

TEST(QuadtreeNode, is_parent_or_leave) {
    // Arrange
    QuadtreeNode node{{{0u,  0u}},
                      {{0.f, 0.f}, {1.f, 1.f}}};

    // Act
    EXPECT_THAT(node.is_leave(), true);
    EXPECT_THAT(node.is_parent(), false);
}

TEST(split_bounds_2f, split_one_time) {
    // Arrange
    AxisAlignedBoundingBox2f bound{{0.f, 0.f}, {2.f, 2.f}};

    // Act
    auto splitted_bounds = split_bound(bound);

    // Assert
    ASSERT_THAT(splitted_bounds.size(), 4);

    EXPECT_THAT(splitted_bounds[0].min_, (Point2f{0.f, 0.f}));
    EXPECT_THAT(splitted_bounds[0].max_, (Point2f{1.f, 1.f}));
    EXPECT_THAT(splitted_bounds[1].min_, (Point2f{1.f, 0.f}));
    EXPECT_THAT(splitted_bounds[1].max_, (Point2f{2.f, 1.f}));
    EXPECT_THAT(splitted_bounds[2].min_, (Point2f{0.f, 1.f}));
    EXPECT_THAT(splitted_bounds[2].max_, (Point2f{1.f, 2.f}));
    EXPECT_THAT(splitted_bounds[3].min_, (Point2f{1.f, 1.f}));
    EXPECT_THAT(splitted_bounds[3].max_, (Point2f{2.f, 2.f}));
}

TEST(split_bounds_2f, split_two_times) {
    // Arrange
    AxisAlignedBoundingBox2f bound{{0.f,   0.f},
                                   {500.f, 500.f}};

    // Act
    auto split1 = split_bound(bound);
    auto split2_0 = split_bound(split1[0]);
    auto split2_1 = split_bound(split1[1]);
    auto split2_2 = split_bound(split1[2]);
    auto split2_3 = split_bound(split1[3]);

    // Assert
    ASSERT_THAT(split1.size(), 4);

    // split1
    EXPECT_THAT(split1[0].min_, (Point2f{0.f, 0.f}));
    EXPECT_THAT(split1[0].max_, (Point2f{250.f, 250.f}));
    EXPECT_THAT(split1[1].min_, (Point2f{250.f, 0.f}));
    EXPECT_THAT(split1[1].max_, (Point2f{500.f, 250.f}));
    EXPECT_THAT(split1[2].min_, (Point2f{0.f, 250.f}));
    EXPECT_THAT(split1[2].max_, (Point2f{250.f, 500.f}));
    EXPECT_THAT(split1[3].min_, (Point2f{250.f, 250.f}));
    EXPECT_THAT(split1[3].max_, (Point2f{500.f, 500.f}));

    // split2_0
    EXPECT_THAT(split2_0[0].min_, (Point2f{0.f, 0.f}));
    EXPECT_THAT(split2_0[0].max_, (Point2f{125.f, 125.f}));
    EXPECT_THAT(split2_0[1].min_, (Point2f{125.f, 0.f}));
    EXPECT_THAT(split2_0[1].max_, (Point2f{250.f, 125.f}));
    EXPECT_THAT(split2_0[2].min_, (Point2f{0.f, 125.f}));
    EXPECT_THAT(split2_0[2].max_, (Point2f{125.f, 250.f}));
    EXPECT_THAT(split2_0[3].min_, (Point2f{125.f, 125.f}));
    EXPECT_THAT(split2_0[3].max_, (Point2f{250.f, 250.f}));

    // split2_1
    EXPECT_THAT(split2_1[0].min_, (Point2f{250.f + 0.f, 0.f}));
    EXPECT_THAT(split2_1[0].max_, (Point2f{250.f + 125.f, 125.f}));
    EXPECT_THAT(split2_1[1].min_, (Point2f{250.f + 125.f, 0.f}));
    EXPECT_THAT(split2_1[1].max_, (Point2f{250.f + 250.f, 125.f}));
    EXPECT_THAT(split2_1[2].min_, (Point2f{250.f + 0.f, 125.f}));
    EXPECT_THAT(split2_1[2].max_, (Point2f{250.f + 125.f, 250.f}));
    EXPECT_THAT(split2_1[3].min_, (Point2f{250.f + 125.f, 125.f}));
    EXPECT_THAT(split2_1[3].max_, (Point2f{250.f + 250.f, 250.f}));

    // split2_2
    EXPECT_THAT(split2_2[0].min_, (Point2f{0.f, 250.f + 0.f}));
    EXPECT_THAT(split2_2[0].max_, (Point2f{125.f, 250.f + 125.f}));
    EXPECT_THAT(split2_2[1].min_, (Point2f{125.f, 250.f + 0.f}));
    EXPECT_THAT(split2_2[1].max_, (Point2f{250.f, 250.f + 125.f}));
    EXPECT_THAT(split2_2[2].min_, (Point2f{0.f, 250.f + 125.f}));
    EXPECT_THAT(split2_2[2].max_, (Point2f{125.f, 250.f + 250.f}));
    EXPECT_THAT(split2_2[3].min_, (Point2f{125.f, 250.f + 125.f}));
    EXPECT_THAT(split2_2[3].max_, (Point2f{250.f, 250.f + 250.f}));

    // split2_3
    EXPECT_THAT(split2_3[0].min_, (Point2f{250.f + 0.f, 250.f + 0.f}));
    EXPECT_THAT(split2_3[0].max_, (Point2f{250.f + 125.f, 250.f + 125.f}));
    EXPECT_THAT(split2_3[1].min_, (Point2f{250.f + 125.f, 250.f + 0.f}));
    EXPECT_THAT(split2_3[1].max_, (Point2f{250.f + 250.f, 250.f + 125.f}));
    EXPECT_THAT(split2_3[2].min_, (Point2f{250.f + 0.f, 250.f + 125.f}));
    EXPECT_THAT(split2_3[2].max_, (Point2f{250.f + 125.f, 250.f + 250.f}));
    EXPECT_THAT(split2_3[3].min_, (Point2f{250.f + 125.f, 250.f + 125.f}));
    EXPECT_THAT(split2_3[3].max_, (Point2f{250.f + 250.f, 250.f + 250.f}));
}

TEST(build_quadtree, GivenNoTriangles_WhenBuildingQuadtree_ExpectNullptrRoot) {
    class NullTriangle2IndexProvider : public IShapesSubBoundsProvider2f {
    public:
        [[nodiscard]]
        AxisAlignedBoundingBox2f bounds() const override {
            return AxisAlignedBoundingBox2f{Point2f{0.f, 0.f}, Point2f{0.f, 0.f}};
        }

        [[nodiscard]]
        AxisAlignedBoundingBox2f bounds(const SubShapeIdentifier &id) const override {
            return AxisAlignedBoundingBox2f{Point2f{0.f, 0.f}, Point2f{0.f, 0.f}};
        }

        [[nodiscard]]
        std::vector<SubShapeIdentifier> sub_shape_ids() const override {
            return std::vector<SubShapeIdentifier>{};
        };
    };

    NullTriangle2IndexProvider tip;
    const size_t MAX_LEAVE_SIZE = 4;

    QuadtreeBuildDescription qtbd{
            &tip,
            tip.bounds(),
            tip.sub_shape_ids(),
            MAX_LEAVE_SIZE
    };

    auto *root = build_quadtree(qtbd);
    EXPECT_THAT(root, nullptr);
}

TEST(build_quadtree, GivenSingleTriangle_WhenBuildingQuadtree_ExpectRootAsLeave) {
    class SingleTriangle2IndexProvider : public IShapesSubBoundsProvider2f {
    public:
        [[nodiscard]]
        AxisAlignedBoundingBox2f bounds() const override {
            return AxisAlignedBoundingBox2f{Point2f{0.f, 0.f}, Point2f{10.f, 10.f}};
        }

        [[nodiscard]]
        AxisAlignedBoundingBox2f bounds(const SubShapeIdentifier &id) const override {
            if (id.parent_shape_index == 0 && id.sub_shape_index == 0) {
                return AxisAlignedBoundingBox2f{Point2f{0.f, 0.f}, Point2f{10.f, 10.f}};
            } else {
                return AxisAlignedBoundingBox2f{Point2f{0.f, 0.f}, Point2f{0.f, 0.f}};
            }
        }

        [[nodiscard]]
        std::vector<SubShapeIdentifier> sub_shape_ids() const override {
            return std::vector<SubShapeIdentifier>{{0, 0}};
        };
    };

    SingleTriangle2IndexProvider tip;
    const size_t MAX_LEAVE_SIZE = 4;

    auto sub_shape_ids = tip.sub_shape_ids();

    QuadtreeBuildDescription qtbd{
            &tip,
            tip.bounds(),
            sub_shape_ids,
            MAX_LEAVE_SIZE
    };
    auto *root = build_quadtree(qtbd);
    EXPECT_FALSE(root == nullptr);

    EXPECT_THAT(root->bounds.min_, (Point2f{0.f, 0.f}));
    EXPECT_THAT(root->bounds.max_, (Point2f{10.f, 10.f}));
    ASSERT_THAT(root->sub_shape_ids.size(), 1);
    EXPECT_THAT(root->sub_shape_ids[0].parent_shape_index, 0);
    EXPECT_THAT(root->sub_shape_ids[0].sub_shape_index, 0);
    EXPECT_THAT(root->nodes[0], nullptr);
    EXPECT_THAT(root->nodes[1], nullptr);
    EXPECT_THAT(root->nodes[2], nullptr);
    EXPECT_THAT(root->nodes[3], nullptr);
}

TEST(build_quadtree, Given4Triangles_WhenBuildingQuadtree_ExpectCorrectQuadtree) {
    class FourTriangle2IndexProvider : public IShapesSubBoundsProvider2f {
    public:
        [[nodiscard]]
        AxisAlignedBoundingBox2f bounds() const override {
            return AxisAlignedBoundingBox2f{Point2f{0.f, 0.f}, Point2f{200.f, 200.f}};
        }

        [[nodiscard]]
        AxisAlignedBoundingBox2f bounds(const SubShapeIdentifier &id) const override {
            assert(id.sub_shape_index < 4);
            assert(id.parent_shape_index == 0);

            if (id.sub_shape_index > 4 || id.parent_shape_index != 0) {
                return AxisAlignedBoundingBox2f{Point2f{0.f, 0.f}, Point2f{0.f, 0.f}};
            }

            std::vector<AxisAlignedBoundingBox2f> bounding_boxes = {
                    AxisAlignedBoundingBox2f{Point2f{0.f, 0.f}, Point2f{100.f, 100.f}},
                    AxisAlignedBoundingBox2f{Point2f{100.f, 0.f}, Point2f{200.f, 100.f}},
                    AxisAlignedBoundingBox2f{Point2f{0.f, 100.f}, Point2f{100.f, 200.f}},
                    AxisAlignedBoundingBox2f{Point2f{100.f, 100.f}, Point2f{200.f, 200.f}},
            };

            return bounding_boxes.at(id.sub_shape_index);
        }

        [[nodiscard]]
        std::vector<SubShapeIdentifier> sub_shape_ids() const override {
            return std::vector<SubShapeIdentifier>{{0, 0},
                                                   {0, 1},
                                                   {0, 2},
                                                   {0, 3}};
        };
    };

    FourTriangle2IndexProvider tip;
    const size_t MAX_LEAVE_SIZE = 4;

    auto sub_shape_ids = tip.sub_shape_ids();

    QuadtreeBuildDescription qtbd{
            &tip,
            tip.bounds(),
            sub_shape_ids,
            MAX_LEAVE_SIZE
    };
    auto *root = build_quadtree(qtbd);
    EXPECT_FALSE(root == nullptr);

    ASSERT_THAT(root->sub_shape_ids.size(), 4);
    EXPECT_THAT(root->sub_shape_ids[0].parent_shape_index, 0);
    EXPECT_THAT(root->sub_shape_ids[1].parent_shape_index, 0);
    EXPECT_THAT(root->sub_shape_ids[2].parent_shape_index, 0);
    EXPECT_THAT(root->sub_shape_ids[3].parent_shape_index, 0);
    EXPECT_THAT(root->sub_shape_ids[0].sub_shape_index, 0);
    EXPECT_THAT(root->sub_shape_ids[1].sub_shape_index, 1);
    EXPECT_THAT(root->sub_shape_ids[2].sub_shape_index, 2);
    EXPECT_THAT(root->sub_shape_ids[3].sub_shape_index, 3);
    EXPECT_THAT(root->nodes[0], nullptr);
    EXPECT_THAT(root->nodes[1], nullptr);
    EXPECT_THAT(root->nodes[2], nullptr);
    EXPECT_THAT(root->nodes[3], nullptr);
}

TEST(build_quadtree, Given4Triangles_WhenBuildingQuadtree_ExpectCorrectQuadtree_MaxLeaveSize1) {
    class FourTriangle2IndexProvider : public IShapesSubBoundsProvider2f {
    public:
        [[nodiscard]]
        AxisAlignedBoundingBox2f bounds() const override {
            // the bounding box is bigger by intention to ease debugging
            return AxisAlignedBoundingBox2f{Point2f{0.f, 0.f}, Point2f{100.f, 100.f}};
        }

        [[nodiscard]]
        AxisAlignedBoundingBox2f bounds(const SubShapeIdentifier &id) const override {
            assert(id.parent_shape_index == 0);
            assert(id.sub_shape_index < 4);
            if (id.parent_shape_index != 0 || id.sub_shape_index > 4) {
                return AxisAlignedBoundingBox2f{Point2f{0.f, 0.f}, Point2f{0.f, 0.f}};
            }

            std::vector<AxisAlignedBoundingBox2f> bounding_boxes = {
                    AxisAlignedBoundingBox2f{Point2f{1.f, 1.f}, Point2f{49.f, 49.f}},
                    AxisAlignedBoundingBox2f{Point2f{51.f, 1.f}, Point2f{99.f, 49.f}},
                    AxisAlignedBoundingBox2f{Point2f{1.f, 51.f}, Point2f{49.f, 99.f}},
                    AxisAlignedBoundingBox2f{Point2f{51.f, 51.f}, Point2f{99.f, 99.f}},
            };

            return bounding_boxes.at(id.sub_shape_index);
        }

        [[nodiscard]]
        std::vector<SubShapeIdentifier> sub_shape_ids() const override {
            return std::vector<SubShapeIdentifier>{{0, 0},
                                                   {0, 1},
                                                   {0, 2},
                                                   {0, 3}};
        };
    };

    FourTriangle2IndexProvider tip;
    const size_t MAX_LEAVE_SIZE = 1;

    auto sub_shape_ids = tip.sub_shape_ids();

    QuadtreeBuildDescription qtbd{
            &tip,
            tip.bounds(),
            sub_shape_ids,
            MAX_LEAVE_SIZE
    };
    auto *root = build_quadtree(qtbd);

    // Assert
    ASSERT_TRUE(root);
    ASSERT_TRUE(root->nodes[0]);
    ASSERT_TRUE(root->nodes[1]);
    ASSERT_TRUE(root->nodes[2]);
    ASSERT_TRUE(root->nodes[3]);
    EXPECT_THAT(root->sub_shape_ids.size(), 0);
    EXPECT_THAT(root->bounds, tip.bounds());

    ASSERT_THAT(root->nodes[0]->sub_shape_ids.size(), 1);
    ASSERT_THAT(root->nodes[1]->sub_shape_ids.size(), 1);
    ASSERT_THAT(root->nodes[2]->sub_shape_ids.size(), 1);
    ASSERT_THAT(root->nodes[3]->sub_shape_ids.size(), 1);

    EXPECT_THAT(root->nodes[0]->sub_shape_ids[0].sub_shape_index, 0);
    EXPECT_THAT(root->nodes[1]->sub_shape_ids[0].sub_shape_index, 1);
    EXPECT_THAT(root->nodes[2]->sub_shape_ids[0].sub_shape_index, 2);
    EXPECT_THAT(root->nodes[3]->sub_shape_ids[0].sub_shape_index, 3);

    EXPECT_THAT(root->nodes[0]->nodes[0], nullptr);
    EXPECT_THAT(root->nodes[0]->nodes[1], nullptr);
    EXPECT_THAT(root->nodes[0]->nodes[2], nullptr);
    EXPECT_THAT(root->nodes[0]->nodes[3], nullptr);

    EXPECT_THAT(root->nodes[1]->nodes[0], nullptr);
    EXPECT_THAT(root->nodes[1]->nodes[1], nullptr);
    EXPECT_THAT(root->nodes[1]->nodes[2], nullptr);
    EXPECT_THAT(root->nodes[1]->nodes[3], nullptr);

    EXPECT_THAT(root->nodes[2]->nodes[0], nullptr);
    EXPECT_THAT(root->nodes[2]->nodes[1], nullptr);
    EXPECT_THAT(root->nodes[2]->nodes[2], nullptr);
    EXPECT_THAT(root->nodes[2]->nodes[3], nullptr);

    EXPECT_THAT(root->nodes[3]->nodes[0], nullptr);
    EXPECT_THAT(root->nodes[3]->nodes[1], nullptr);
    EXPECT_THAT(root->nodes[3]->nodes[2], nullptr);
    EXPECT_THAT(root->nodes[3]->nodes[3], nullptr);
}

TEST(QuadtreeIntersector, square_box) {
    auto aabb = AxisAlignedBoundingBox2f{Point2f{0.f, 0.f}, Point2f{2.f, 1.f}};

    auto squared_aabb = QuadtreeIntersector::square_box(aabb);

    EXPECT_THAT(squared_aabb.width(), squared_aabb.height());
    auto union_box = union_aabb(squared_aabb, aabb);
    EXPECT_THAT(union_box, squared_aabb);
}

TEST(QuadtreeIntersector, build_acceleration_structure) {
    // Arrange

    /// Create a triangle mesh
    auto transform = identity<float>();
    PropertySet tm_ps;
    tm_ps.add_property("parent_path", std::string("flatland/scenes/terrain"));
    tm_ps.add_property("transform", transform);
    tm_ps.add_property("filename", std::string("small_terrain.obj2d"));
    ReferenceCounted<TriangleMesh2f> tm = make_reference_counted<TriangleMesh2f>(tm_ps);

    // Act
    PropertySet ps{};
    QuadtreeIntersector intersector{ps};
    std::vector<ReferenceCounted<Shape2f>> shapes = {tm};
    intersector.build_acceleration_structure(shapes);

    QuadtreeNode *root = intersector.root_node();

    // Assert
    ASSERT_TRUE(root);

    for (int i = 0; i < 4; ++i) {
        EXPECT_THAT(root->nodes[i], nullptr);
    }
}

TEST(QuadtreeIntersector, to_string) {
    PropertySet ps{};
    QuadtreeIntersector intersector{ps};

    EXPECT_THAT(intersector.to_string(), "QuadtreeIntersector");
}

TEST(QuadtreeIntersector, invalid_strategy) {
    PropertySet ps;
    ps.add_property("strategy", std::string("unknown_invalid_not_existing_strategy"));

    EXPECT_THROW(QuadtreeIntersector{ps}, std::runtime_error);
}

TEST(QuadtreeIntersector, GivenEmptyScene_WhenIntersection_ExpectNoIntersection) {
    // Arrange
    std::vector<ReferenceCounted<Shape2f>> shapes = {};
    PropertySet ps{};
    QuadtreeIntersector intersector{ps};
    intersector.build_acceleration_structure(shapes);

    // Act
    Ray2f ray{{0.f, 50.f}, {1.f, 0.f}, 0.f, 1000.f};
    MediumEvent2f me;
    bool hit = intersector.intersect(ray, me);

    // Assert
    ASSERT_FALSE(hit);
}

TEST(QuadtreeIntersector, GivenASceneAndANonIntersectionRay_WhenComputingIntersection_ThenExpectNoIntersection) {
    // Arrange
    /// Create a triangle mesh
    auto transform = identity<float>();
    PropertySet tm_ps;
    tm_ps.add_property("parent_path", std::string("flatland/scenes/quadtree"));
    tm_ps.add_property("transform", transform);
    tm_ps.add_property("filename", std::string("test3.obj2d"));
    ReferenceCounted<TriangleMesh2f> tm = std::make_shared<TriangleMesh2f>(tm_ps);

    PropertySet ps{};
    QuadtreeIntersector intersector{ps};
    std::vector<ReferenceCounted<Shape2f>> shapes = {tm};
    intersector.build_acceleration_structure(shapes);

    // Act
    Ray2f ray{{0.f, 50.f}, {-1.f, 0.f}, 0.f, 1000.f};
    MediumEvent2f me;
    bool hit = intersector.intersect(ray, me);

    // Assert
    ASSERT_FALSE(hit);
}

TEST(QuadtreeIntersector, intersect) {
    // Arrange
    /// Create a triangle mesh
    auto transform = identity<float>();
    PropertySet tm_ps;
    tm_ps.add_property("parent_path", std::string("flatland/scenes/quadtree"));
    tm_ps.add_property("transform", transform);
    tm_ps.add_property("filename", std::string("test3.obj2d"));
    ReferenceCounted<TriangleMesh2f> tm = std::make_shared<TriangleMesh2f>(tm_ps);

    PropertySet ps{};
    QuadtreeIntersector intersector{ps};
    std::vector<ReferenceCounted<Shape2f>> shapes = {tm};
    intersector.build_acceleration_structure(shapes);

    // Act
    Ray2f ray{{0.f, 50.f}, {1.f, 0.f}, 0.f, 1000.f};
    MediumEvent2f me;
    bool hit = intersector.intersect(ray, me);

    // Assert
    ASSERT_TRUE(hit);
    EXPECT_THAT(me.p.x(), 100.0f);
    EXPECT_THAT(me.p.y(), 50.0f);
    EXPECT_THAT(me.t, 100.f);
    EXPECT_THAT(me.geo_frame.normal.x(), -1.f);
    EXPECT_THAT(me.geo_frame.normal.y(), 0.f);
}

TEST(QuadtreeIntersector, support_translated_meshes) {
    // Arrange

    /// Create a triangle mesh
    auto transform = translate<float>(100, 100, 0);
    PropertySet tm_ps;
    tm_ps.add_property("parent_path", std::string("flatland/scenes/quadtree"));
    tm_ps.add_property("transform", transform);
    tm_ps.add_property("filename", std::string("single_triangle.obj2d"));
    ReferenceCounted<TriangleMesh2f> tm = std::make_shared<TriangleMesh2f>(tm_ps);

    // Act
    PropertySet ps{};
    QuadtreeIntersector intersector{ps};
    std::vector<ReferenceCounted<Shape2f>> shapes = {tm};
    intersector.build_acceleration_structure(shapes);

    // Assert
    EXPECT_THAT(intersector.root_node()->bounds.min_.x(), ::testing::Ge(100));
    EXPECT_THAT(intersector.root_node()->bounds.min_.y(), ::testing::Ge(100));
}
