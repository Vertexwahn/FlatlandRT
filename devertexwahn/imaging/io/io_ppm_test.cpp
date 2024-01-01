/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/io/io_ppm.h"

#include <fstream>

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(Image, storeLoadOpenPPM) {
    // Arrange
    Image4b image{100, 100};

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            image.set_pixel(x, y, Color4b{255, 128, 0, 255});
        }
    }

    // Act
    store_ppm("test.ppm", image);

    // Assert
    std::ifstream ppm_file{"test.ppm"};
    std::string line;

    std::getline(ppm_file, line);
    EXPECT_THAT(line, "P3");

    std::getline(ppm_file, line);
    EXPECT_THAT(line, "100 100");

    std::getline(ppm_file, line);
    EXPECT_THAT(line, "255");

    std::getline(ppm_file, line);
    EXPECT_THAT(line, testing::HasSubstr("255 128 0"));
}
