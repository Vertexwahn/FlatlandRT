/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/math/geometry.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(Point2f, GivenPointsInCounterClockwiseOrder_WhenIsClockwiseOrder_ThenClockwiseOrderIsFalse) {
    std::vector<Point2f> points = {
            {-50.0f, -50.0f},
            {50.0f, -50.0f},
            {50.0f, 50.0f},
            {-50.0f, 50.0f}
    };

    EXPECT_FALSE(isClockwiseOrder(points));
}

TEST(Point2f, GivenPointsInClockwiseOrder_WhenIsClockwiseOrder_ThenClockwiseOrderIsTrue) {
    std::vector<Point2f> points = {
            {0.0f, 0.0f},
            {1.0f, 1.0f},
            {1.0f, 0.0f},
    };

    EXPECT_TRUE(isClockwiseOrder(points));
}

TEST(Point2f, GivenPointsInCounterClockwiseOrder_WhenIsCounterClockwiseOrder_ThenCounterClockwiseOrderIsTrue) {
    std::vector<Point2f> points = {
            {-50.0f, -50.0f},
            {50.0f, -50.0f},
            {50.0f, 50.0f},
            {-50.0f, 50.0f}
    };

    EXPECT_TRUE(isCounterClockwiseOrder(points));
}

TEST(Point2f, GivenPointsInClockwiseOrder_WhenIsCounterClockwiseOrder_ThenCounterClockwiseOrderIsFalse) {
    std::vector<Point2f> points = {
            {0.0f, 0.0f},
            {1.0f, 1.0f},
            {1.0f, 0.0f},
    };

    EXPECT_FALSE(isCounterClockwiseOrder(points));
}

TEST(Point2f, isCounterClockwiseOrder) {
    std::vector<Point2f> points = {
            {0.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 15.0f}
    };

    EXPECT_TRUE(isCounterClockwiseOrder(points));
}

TEST(createTrianglePoints, WhenCreatingTrianglePoints_ThenExpectCounterClockwiseOrder) {
    auto points = createTrianglePoints<float>();
    EXPECT_TRUE(isCounterClockwiseOrder(points));
}

TEST(createSquare, WhenCreatingSquarePoints_ThenExpectCounterClockwiseOrder) {
    auto points = createSquare(100.0f);
    EXPECT_TRUE(isCounterClockwiseOrder(points));
    EXPECT_THAT(points[0], Point2f(-50.0f, -50.0f));
    EXPECT_THAT(points[1], Point2f( 50.0f, -50.0f));
    EXPECT_THAT(points[2], Point2f( 50.0f,  50.0f));
    EXPECT_THAT(points[3], Point2f(-50.0f,  50.0f));
}

TEST(createStanfordBunnyPointList, createStanfordBunnyPointList) {
    auto points = createStanfordBunnyPointList<float>();
    EXPECT_TRUE(isCounterClockwiseOrder(points));
}

TEST(read2DPlyFile, read2DPlyFile) {
    auto points = read2DPlyFile<float>("scenes/bunny_coords.ply");

    EXPECT_THAT(points.size(), testing::Ge(170));

    EXPECT_NEAR(points[points.size()-1].x(), -40.5615654f, 0.0001f);
    EXPECT_NEAR(points[points.size()-1].y(), 38.51184082f, 0.0001f);
}
