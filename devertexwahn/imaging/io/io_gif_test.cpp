/*
 *  SPDX-FileCopyrightText: Copyright 2026 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/io/io_gif.hpp"

#include "gmock/gmock.h"

#include <filesystem>

using namespace de_vertexwahn;

TEST(Image, roundtrip_store_load_gif_4x4_with_colors) {
    // Arrange - Create a 4x4 image with red, green, blue, and white pixels
    Image4b image{4, 4};
    
    // Set red pixel at (0, 0)
    image.set_pixel(0, 0, ColorRGBA4b{255, 0, 0, 255});
    // Set green pixel at (1, 0)
    image.set_pixel(1, 0, ColorRGBA4b{0, 255, 0, 255});
    // Set blue pixel at (2, 0)
    image.set_pixel(2, 0, ColorRGBA4b{0, 0, 255, 255});
    // Set white pixel at (3, 0)
    image.set_pixel(3, 0, ColorRGBA4b{255, 255, 255, 255});
    
    // Fill remaining pixels with black
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (y > 0 || x > 3) {
                image.set_pixel(x, y, ColorRGBA4b{0, 0, 0, 255});
            }
        }
    }

    // Act - Store and load the GIF
    const char* filename = "test_roundtrip_4x4.gif";
    bool store_result = store_gif(filename, image);
    ASSERT_TRUE(store_result);

    Image3f loaded_image = load_image_gif(filename);

    // Assert - Check dimensions
    EXPECT_EQ(loaded_image.width(), 4);
    EXPECT_EQ(loaded_image.height(), 4);

    // Check red pixel at (0, 0)
    auto red_pixel = loaded_image.get_pixel(0, 0);
    EXPECT_FLOAT_EQ(red_pixel.red(), 1.0f);
    EXPECT_FLOAT_EQ(red_pixel.green(), 0.0f);
    EXPECT_FLOAT_EQ(red_pixel.blue(), 0.0f);

    // Check green pixel at (1, 0)
    auto green_pixel = loaded_image.get_pixel(1, 0);
    EXPECT_FLOAT_EQ(green_pixel.red(), 0.0f);
    EXPECT_FLOAT_EQ(green_pixel.green(), 1.0f);
    EXPECT_FLOAT_EQ(green_pixel.blue(), 0.0f);

    // Check blue pixel at (2, 0)
    auto blue_pixel = loaded_image.get_pixel(2, 0);
    EXPECT_FLOAT_EQ(blue_pixel.red(), 0.0f);
    EXPECT_FLOAT_EQ(blue_pixel.green(), 0.0f);
    EXPECT_FLOAT_EQ(blue_pixel.blue(), 1.0f);

    // Check white pixel at (3, 0)
    auto white_pixel = loaded_image.get_pixel(3, 0);
    EXPECT_FLOAT_EQ(white_pixel.red(), 1.0f);
    EXPECT_FLOAT_EQ(white_pixel.green(), 1.0f);
    EXPECT_FLOAT_EQ(white_pixel.blue(), 1.0f);

    // Cleanup
    std::filesystem::remove(filename);
}

TEST(Image, store_gif_Image3f) {
    // Arrange - Create a 2x2 Image3f
    Image3f image{2, 2};
    image.set_pixel(0, 0, ColorRGB3f{1.0f, 0.0f, 0.0f});  // Red
    image.set_pixel(1, 0, ColorRGB3f{0.0f, 1.0f, 0.0f});  // Green
    image.set_pixel(0, 1, ColorRGB3f{0.0f, 0.0f, 1.0f});  // Blue
    image.set_pixel(1, 1, ColorRGB3f{1.0f, 1.0f, 1.0f});  // White

    // Act
    const char* filename = "test_Image3f.gif";
    bool store_result = store_gif(filename, image);
    ASSERT_TRUE(store_result);

    Image3f loaded_image = load_image_gif(filename);

    // Assert
    EXPECT_EQ(loaded_image.width(), 2);
    EXPECT_EQ(loaded_image.height(), 2);

    EXPECT_FLOAT_EQ(loaded_image.get_pixel(0, 0).red(), 1.0f);
    EXPECT_FLOAT_EQ(loaded_image.get_pixel(0, 0).green(), 0.0f);
    EXPECT_FLOAT_EQ(loaded_image.get_pixel(0, 0).blue(), 0.0f);

    EXPECT_FLOAT_EQ(loaded_image.get_pixel(1, 0).red(), 0.0f);
    EXPECT_FLOAT_EQ(loaded_image.get_pixel(1, 0).green(), 1.0f);
    EXPECT_FLOAT_EQ(loaded_image.get_pixel(1, 0).blue(), 0.0f);

    EXPECT_FLOAT_EQ(loaded_image.get_pixel(0, 1).red(), 0.0f);
    EXPECT_FLOAT_EQ(loaded_image.get_pixel(0, 1).green(), 0.0f);
    EXPECT_FLOAT_EQ(loaded_image.get_pixel(0, 1).blue(), 1.0f);

    EXPECT_FLOAT_EQ(loaded_image.get_pixel(1, 1).red(), 1.0f);
    EXPECT_FLOAT_EQ(loaded_image.get_pixel(1, 1).green(), 1.0f);
    EXPECT_FLOAT_EQ(loaded_image.get_pixel(1, 1).blue(), 1.0f);

    // Cleanup
    std::filesystem::remove(filename);
}

TEST(Image, try_to_load_non_existing_gif_file) {
    EXPECT_THROW(load_image_gif("non_existing_file.gif"), std::runtime_error);
}
