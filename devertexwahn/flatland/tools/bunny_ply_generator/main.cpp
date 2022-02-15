/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/canvas/svg_canvas.h"
#include "flatland/math/geometry.h"

#include <iomanip>
#include <iostream>

using namespace flatland;

int main(int argc, char **argv) {
    auto points = create_stanford_bunny_point_list<double>();
    auto minV = points[0];
    auto maxV = points[0];
    for (const auto &point : points) {
        if(point.x() < minV.x())
            minV.x() = point.x();
        if(point.y() < minV.y())
            minV.y() = point.y();
        if(point.x() > maxV.x())
            maxV.x() = point.x();
        if(point.y() > maxV.y())
            maxV.y() = point.y();
    }
    auto center = minV + 0.5 * (maxV - minV);
    std::cout << center << std::endl;
    std::string filename = "muh.txt";
    std::ofstream out_file(filename);
    if (!out_file.is_open()) {
        throw std::runtime_error("Failed to open file \"" + filename + "\"");
    }
    for (const auto &point : points) {
        out_file << "v ";
        out_file << std::setprecision(10) << point.x() - center.x();
        out_file << ", ";
        out_file << std::setprecision(10) << point.y() - center.y();
        out_file << std::endl;
    }
    return  0;
}