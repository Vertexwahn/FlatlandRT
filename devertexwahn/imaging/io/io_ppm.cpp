/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/io/io_ppm.h"

#include <fstream>

DE_VERTEXWAHN_BEGIN_NAMESPACE

void store_ppm(std::string_view filename, const Image4b &image) {
    std::ofstream out(filename.data(), std::ios::out);

    out << "P3" << std::endl
        << image.width() << ' ' << image.height() << std::endl
        << "255" << std::endl;

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            Color4b color = image.get_pixel(x, y);
            int ir = color.red();
            int ig = color.green();
            int ib = color.blue();

            out << ir << ' ' << ig << ' ' << ib << std::endl;
        }
    }

    out.close();
}

DE_VERTEXWAHN_END_NAMESPACE
