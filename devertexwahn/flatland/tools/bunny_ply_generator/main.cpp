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
    auto points = create_stanford_bunny_point_list < double > ();

    auto min_value = points[0];
    auto max_value = points[0];

    for (const auto &point: points) {
        if (point.x() < min_value.x())
            min_value.x() = point.x();
        if (point.y() < min_value.y())
            min_value.y() = point.y();
        if (point.x() > max_value.x())
            max_value.x() = point.x();
        if (point.y() > max_value.y())
            max_value.y() = point.y();
    }

    auto center = min_value + 0.5 * (max_value - min_value);

    std::cout << center << std::endl;
    std::string filename = "muh.txt";
    std::ofstream out_file(filename);

    if (!out_file.is_open()) {
        throw std::runtime_error("Failed to open file \"" + filename + "\"");
    }

    for (const auto &point: points) {
        out_file << "v ";
        out_file << std::setprecision(10) << point.x() - center.x();
        out_file << ", ";
        out_file << std::setprecision(10) << point.y() - center.y();
        out_file << std::endl;
    }
    return 0;
}