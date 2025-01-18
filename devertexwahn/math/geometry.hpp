/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Math_Geometry_fba52631_f901_4d31_9fb1_06925a0a0967_h
#define De_Vertexwahn_Math_Geometry_fba52631_f901_4d31_9fb1_06925a0a0967_h

#include "math/point.hpp"

#include "boost/algorithm/string.hpp"
#include "fmt/core.h"

#include <filesystem>
#include <fstream>
#include <string_view>
#include <vector>

DE_VERTEXWAHN_BEGIN_NAMESPACE

template <typename ScalarType>
bool is_counter_clockwise_order(const std::vector<Point2<ScalarType>>& points) {
    return !is_clockwise_order(points);
}

template <typename ScalarType>
bool is_clockwise_order(const std::vector<Point2<ScalarType>>& points) {
    float sum_over_all_edges = ScalarType{0.0};

    size_t point_count = points.size();
    for (size_t i = 0; i < point_count; ++i) {
        ScalarType x1 = points[i].x();
        ScalarType y1 = points[i].y();
        ScalarType x2 = points[(i + 1) % point_count].x();
        ScalarType y2 = points[(i + 1) % point_count].y();

        sum_over_all_edges += (x2 - x1) * (y2 + y1);
    }

    return sum_over_all_edges >= 0;
}

template <typename ScalarType>
std::vector<Point2<ScalarType>> create_triangle_points() {
    std::vector<Point2<ScalarType>> points = {
      Point2<ScalarType>(ScalarType{0.0}, ScalarType{0.0}),
      Point2<ScalarType>(ScalarType{100.f}, ScalarType{0.0}),
      Point2<ScalarType>(ScalarType{50.f}, ScalarType{100.f}),
    };

    return points;
}

template <typename ScalarType>
std::vector<Point2<ScalarType>> create_square(const ScalarType size) {
    ScalarType halfSize = size * ScalarType{0.5f};

    std::vector<Point2<ScalarType>> points = {
      {-halfSize, -halfSize}, {halfSize, -halfSize}, {halfSize, halfSize}, {-halfSize, halfSize}};

    return points;
}

template <typename ScalarType>
std::vector<Point2<ScalarType>> load_2D_ply(std::string_view filename) {
    using Point = Point2<ScalarType>;

    if (!std::filesystem::exists(filename)) {
        throw std::runtime_error(fmt::format("File {} does not exist.", filename));
    }

    std::vector<Point> points;
    std::ifstream file(filename.data());
    std::string line;
    while (std::getline(file, line)) {
        // skip comments
        if (boost::starts_with(line, "#"))
            continue;

        // skip empty lines
        if (line == std::string("") || line == "\r")
            continue;

        std::vector<std::string> tokens;
        boost::split(tokens, line, boost::is_any_of(" "));

        ScalarType x = std::stof(tokens[1]);
        ScalarType y = std::stof(tokens[2]);

        points.push_back(Point2<ScalarType>{x, y});
    }
    return points;
}

template<typename ScalarType>
void load_obj2d(std::string_view filename,
                std::vector<Point2<ScalarType>> &positions,
                std::vector<std::uint32_t> &indices) {
    if (!std::filesystem::exists(filename)) {
        throw std::runtime_error(fmt::format("File {} does not exist.", filename));
    }

    std::ifstream file(filename.data());
    std::string line;
    while (std::getline(file, line)) {
        // skip comments
        if (boost::starts_with(line, "#"))
            continue;

        // skip empty lines
        if (line == std::string(""))
            continue;

        std::vector<std::string> tokens;
        boost::split(tokens, line, boost::is_any_of(" "));

        if(tokens[0] == "v") {
            ScalarType x = std::stof(tokens[1]);
            ScalarType y = std::stof(tokens[2]);

            positions.push_back(Point2f{x, y});
        }

        if(tokens[0] == "f") {
            int index0 = std::stoi(tokens[1]);
            int index1 = std::stoi(tokens[2]);
            int index2 = std::stoi(tokens[3]);

            indices.push_back(index0);
            indices.push_back(index1);
            indices.push_back(index2);
        }
    }
}

#include "math/geometry.inc"

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Math_Geometry_fba52631_f901_4d31_9fb1_06925a0a0967_h
