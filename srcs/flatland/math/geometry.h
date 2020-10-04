/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_geometry_fba52631_f901_4d31_9fb1_06925a0a0967_h
#define Flatland_geometry_fba52631_f901_4d31_9fb1_06925a0a0967_h

#include "flatland/core/namespace.h"
#include "flatland/math/point.h"

#include <vector>

FLATLAND_BEGIN_NAMESPACE

template <typename ScalarType>
bool isCounterClockwiseOrder(const std::vector<Point2<ScalarType>>& points) {
    return !isClockwiseOrder(points);
}

template <typename ScalarType>
bool isClockwiseOrder(const std::vector<Point2<ScalarType>>& points) {
    float sumOverAllEdges = ScalarType{0.0};

    size_t pointCount = points.size();
    for(size_t i = 0; i < pointCount; ++i) {
        ScalarType x1 = points[i].x();
        ScalarType y1 = points[i].y();
        ScalarType x2 = points[(i+1) % pointCount].x();
        ScalarType y2 = points[(i+1) % pointCount].y();

        sumOverAllEdges += (x2 - x1) * (y2 + y1);
    }

    return sumOverAllEdges >= 0;
}

template <typename ScalarType>
std::vector<Point2<ScalarType>> createTrianglePoints() {
    std::vector<Point2<ScalarType>> points = {
            Point2<ScalarType>(ScalarType{0.0}, ScalarType{0.0}),
            Point2<ScalarType>(ScalarType{100.0f}, ScalarType{0.0}),
            Point2<ScalarType>(ScalarType{50.0f}, ScalarType{100.0f}),
    };

    return points;
}

template <typename ScalarType>
std::vector<Point2<ScalarType>> createSquare(const ScalarType size) {
    ScalarType halfSize = size * ScalarType{0.5f};

    std::vector<Point2<ScalarType>> points = {
            {-halfSize, -halfSize},
            {halfSize, -halfSize},
            {halfSize, halfSize},
            {-halfSize, halfSize}
    };

    return points;
}

#include "geometry.inc"

FLATLAND_END_NAMESPACE

#endif // end define Flatland_geometry_fba52631_f901_4d31_9fb1_06925a0a0967_h
