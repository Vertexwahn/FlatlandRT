/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "gmock/gmock.h"

#include "flatland/rendering/scene/shape/triangle_mesh.h"

using namespace flatland;

TEST(TriangleMesh2, GivenPointWithInfiniteValue_WhenConstructionTriangleMesh_ThenExpectException) {
    // Arrange
    std::vector<Point2f> points = {
            {0.f,                                    0.f},
            {1.f,                                    0.f},
            {std::numeric_limits<float>::infinity(), 1.f}
    };

    std::vector<std::uint32_t> indices = {0, 1, 2};

    auto transform = identity<float>();

    PropertySet ps;
    ps.add_property("transform", transform);

    // Act
    EXPECT_THROW((TriangleMesh2f{ps, points, indices}), std::runtime_error);

    // Assert
    auto try_to_construct_with_invalid_points = [&]() {
        TriangleMesh2f{ps, points, indices};
    };

    EXPECT_THAT([&]() { try_to_construct_with_invalid_points(); },
                testing::ThrowsMessage<std::runtime_error>(testing::HasSubstr("x component of point is not finite")));
}

TEST(TriangleMesh2, GivenPointInClockwiseOrder_WhenConstructionTriangleMesh_ThenExpectException) {
    // Arrange
    std::vector<Point2f> points = {
            {0.f, 0.f}, // A
            {1.f, 1.f}, // C
            {1.f, 0.f}, // B
    };

    std::vector<std::uint32_t> indices = {0, 1, 2};

    auto transform = identity<float>();

    PropertySet ps;
    ps.add_property("transform", transform);

    // Act
    EXPECT_THROW((TriangleMesh2f{ps, points, indices}), std::runtime_error);
}

TEST(TriangleMesh2, GivenInvalidIndices_WhenConstructionTriangleMesh_ThenExpectException) {
    // Arrange
    std::vector<Point2f> points = {
            {0.f, 0.f},
            {1.f, 0.f},
            {1.f, 1.f}
    };

    std::vector<std::uint32_t> indices = {1, 2, 3};

    auto transform = identity<float>();

    PropertySet ps;
    ps.add_property("transform", transform);

    // Act
    EXPECT_THROW((TriangleMesh2f{ps, points, indices}), std::runtime_error);
}

TEST(TriangleMesh2, load_ctor_via_obj2d) {
    // Arrange
    auto transform = translate(0.f, 0.f, 0.f);

    PropertySet ps;
    ps.add_property("parent_path", std::string("flatland/scenes/terrain"));
    ps.add_property("transform", transform);
    ps.add_property("filename", std::string("terrain.obj2d"));

    // Act
    TriangleMesh2f tm{ps};

    // Assert
    EXPECT_THAT(tm.index_count(), 600);
    EXPECT_THAT(tm.position_count(), 400);
}

TEST(TriangleMesh2, GivenTranslationTransform_WhenConstructionAMesh_ThenExpectTranslatedMesh) {
    // Arrange
    auto transform = translate(1.f, 2.f, 3.f);

    PropertySet ps;
    ps.add_property("parent_path", std::string("flatland/scenes/terrain"));
    ps.add_property("transform", transform);
    ps.add_property("filename", std::string("terrain.obj2d"));

    // Act
    TriangleMesh2f tm{ps};

    // Assert
    auto actual_transform = tm.transform();

    Matrix44f expected_matrix;
    expected_matrix << 1.f, 0.f, 0.f, 1.f,
            0.f, 1.f, 0.f, 2.f,
            0.f, 0.f, 1.f, 3.f,
            0.f, 0.f, 0.f, 1.f;

    EXPECT_THAT(actual_transform.matrix(), expected_matrix);
}

TEST(TriangleMesh2, GivenTriangle_WhenComputingBound_ExpectCorrectBounds) {
    // Arrange
    std::vector<Point2f> points = {
            {0.f, 0.f},
            {1.f, 0.f},
            {1.f, 1.f}
    };

    std::vector<std::uint32_t> indices = {0, 1, 2};

    auto transform = identity<float>();

    PropertySet ps;
    ps.add_property("transform", transform);

    TriangleMesh2f tm{ps, points, indices};

    // Act
    auto bounds = tm.bounds();

    // Assert
    EXPECT_THAT(bounds.min_.x(), 0.f);
    EXPECT_THAT(bounds.min_.y(), 0.f);
    EXPECT_THAT(bounds.max_.y(), 1.f);
    EXPECT_THAT(bounds.max_.y(), 1.f);
};

