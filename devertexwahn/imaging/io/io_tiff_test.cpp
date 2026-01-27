/*
 *  SPDX-FileCopyrightText: Copyright 2026 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/io/io_tiff.hpp"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(Tiff, roundtrip_store_and_load) {
    // Arrange - create a 2x2 image with red, green, blue, white
    Image4b image{2, 2};
    image.set_pixel(0, 0, ColorRGBA4b{255, 0, 0, 255});     // Red
    image.set_pixel(1, 0, ColorRGBA4b{0, 255, 0, 255});     // Green
    image.set_pixel(0, 1, ColorRGBA4b{0, 0, 255, 255});     // Blue
    image.set_pixel(1, 1, ColorRGBA4b{255, 255, 255, 255}); // White

    // Act - store and load
    bool store_result = store_tiff("roundtrip_test.tiff", image);
    ASSERT_TRUE(store_result);

    auto loaded_image = load_image_tiff("roundtrip_test.tiff");

    // Assert
    ASSERT_NE(loaded_image, nullptr);
    EXPECT_EQ(loaded_image->width(), 2);
    EXPECT_EQ(loaded_image->height(), 2);

    EXPECT_THAT(loaded_image->get_pixel(0, 0), (ColorRGBA4b{255, 0, 0, 255}));     // Red
    EXPECT_THAT(loaded_image->get_pixel(1, 0), (ColorRGBA4b{0, 255, 0, 255}));     // Green
    EXPECT_THAT(loaded_image->get_pixel(0, 1), (ColorRGBA4b{0, 0, 255, 255}));     // Blue
    EXPECT_THAT(loaded_image->get_pixel(1, 1), (ColorRGBA4b{255, 255, 255, 255})); // White
}

TEST(Tiff, load_non_existing_file) {
    auto image = load_image_tiff("non_existing_file.tiff");
    EXPECT_EQ(image, nullptr);
}
