/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "math/geometry.hpp"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(Point2f, GivenPointsInCounterClockwiseOrder_WhenIsClockwiseOrder_ThenClockwiseOrderIsFalse) {
    std::vector<Point2f> points = {
            {-50.f, -50.f},
            {50.f,  -50.f},
            {50.f,  50.f},
            {-50.f, 50.f}
    };

    EXPECT_FALSE(is_clockwise_order(points));
}

TEST(Point2f, GivenPointsInClockwiseOrder_WhenIsClockwiseOrder_ThenClockwiseOrderIsTrue) {
    std::vector<Point2f> points = {
            {0.f, 0.f},
            {1.f, 1.f},
            {1.f, 0.f},
    };

    EXPECT_TRUE(is_clockwise_order(points));
}

TEST(Point2f, GivenPointsInCounterClockwiseOrder_WhenIsCounterClockwiseOrder_ThenCounterClockwiseOrderIsTrue) {
    std::vector<Point2f> points = {
            {-50.f, -50.f},
            {50.f,  -50.f},
            {50.f,  50.f},
            {-50.f, 50.f}
    };

    EXPECT_TRUE(is_counter_clockwise_order(points));
}

TEST(Point2f, GivenPointsInClockwiseOrder_WhenIsCounterClockwiseOrder_ThenCounterClockwiseOrderIsFalse) {
    std::vector<Point2f> points = {
            {0.f, 0.f},
            {1.f, 1.f},
            {1.f, 0.f},
    };

    EXPECT_FALSE(is_counter_clockwise_order(points));
}

TEST(Point2f, is_counter_clockwise_order) {
    std::vector<Point2f> points = {
            {0.f, 0.f},
            {1.f, 1.f},
            {0.f, 15.f}
    };

    EXPECT_TRUE(is_counter_clockwise_order(points));
}

TEST(create_triangle_points, WhenCreatingTrianglePoints_ThenExpectCounterClockwiseOrder) {
    auto points = create_triangle_points<float>();
    EXPECT_TRUE(is_counter_clockwise_order(points));
}

TEST(create_square, WhenCreatingSquarePoints_ThenExpectCounterClockwiseOrder) {
    auto points = create_square(100.f);
    EXPECT_TRUE(is_counter_clockwise_order(points));
    EXPECT_THAT(points[0], Point2f(-50.f, -50.f));
    EXPECT_THAT(points[1], Point2f(50.f, -50.f));
    EXPECT_THAT(points[2], Point2f(50.f, 50.f));
    EXPECT_THAT(points[3], Point2f(-50.f, 50.f));
}

TEST(create_stanford_bunny_point_list, create_stanford_bunny_point_list) {
    auto points = create_stanford_bunny_point_list < float > ();
    EXPECT_TRUE(is_counter_clockwise_order(points));
}

TEST(load_2D_ply, WhenRedingBunnyFile_ExpectValidCoordinates) {
    auto points = load_2D_ply<float>("flatland/scenes/bunny/bunny_coords.ply");

    EXPECT_THAT(points.size(), testing::Ge(170));

    EXPECT_NEAR(points[points.size() - 1].x(), -40.5615654f, 0.0001f);
    EXPECT_NEAR(points[points.size() - 1].y(), 38.51184082f, 0.0001f);
}

TEST(load_2D_ply, WhenFileDoesNotExiten_ThenThrowException) {
    EXPECT_THROW(load_2D_ply<float>("file_that_does_not_exist"), std::runtime_error);
}

TEST(load_obj2d, WhenFileDoesNotExiten_ThenThrowException) {
    // Arrange
    std::vector<Point2f> positions;
    std::vector<std::uint32_t> indices;

    // Act
    EXPECT_THROW(load_obj2d<float>("file_that_does_not_exist", positions, indices), std::runtime_error);
}

TEST(load_obj2d, WhenRedingTerrainFile_ExpectValidCoordinatesAndIndices) {
    // Arrange
    std::vector<Point2f> positions;
    std::vector<std::uint32_t> indices;

    // Act
    load_obj2d<float>("flatland/scenes/terrain/terrain.obj2d", positions, indices);

    // Assert
    EXPECT_THAT(positions.size(), testing::Ge(300));
    EXPECT_THAT(indices.size(), testing::Ge(1));

    EXPECT_NEAR(positions[positions.size() - 1].x(), 450.f, 500.f);
    EXPECT_THAT(indices[indices.size() - 1], 396);
}
