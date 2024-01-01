/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/io/io_png.h"

#include "gmock/gmock.h"

#include <fstream>

using namespace de_vertexwahn;

TEST(Image4f, convert_to_Image4f) {
    Image4b source_image{2, 2, Color4b{0, 128, 255, 255}};
    store_png("source.png", source_image);
    ReferenceCounted<Image4f> dest_image = load_image_png_as_Image4f("source.png");
    EXPECT_THAT(dest_image->get_pixel(0,0), (Color4f{0.f, 128.0f/255.f, 1.f, 1.f}));
}

TEST(Image, store_png) {
    // Arrange
    Image4b image{2, 2};
    image.set_pixel(0, 0, Color4b{255, 0, 0, 255});
    image.set_pixel(1, 0, Color4b{0, 255, 0, 255});
    image.set_pixel(0, 1, Color4b{0, 0, 255, 255});
    image.set_pixel(1, 1, Color4b{255, 128, 0, 255});

    // Act
    store_png("test4x4.png", image);

    // Assert
    auto loaded_image = load_image_png("test4x4.png");

    EXPECT_THAT(loaded_image->get_pixel(0, 0), (Color3f{1.f, 0.f, 0.f}));
    //EXPECT_THAT(loaded_image->get_pixel(1, 0), (Color3f{0.f, 1.f, 0.f}));
    //EXPECT_THAT(loaded_image->get_pixel(0, 1), (Color3f{0.f, 0.f, 1.f}));
    //EXPECT_THAT(loaded_image->get_pixel(1, 1), (Color3f{1.f, .5f, 0.f}));
}

TEST(Image, try_to_load_non_existing_png_file) {
    EXPECT_THAT(load_image_png("non_existing_file.png"), nullptr);
}

TEST(Image, try_to_load_invalid_png_file) {
    std::ofstream file("invalid.png");
    file << "invalid";
    file.close();

    EXPECT_THAT(load_image_png("invalid.png"), nullptr);
}

TEST(Image, load_RGBA_png_file) {
    auto image = load_image_png("imaging/tests/data/RGBA.png");

    EXPECT_THAT(image->get_pixel(0,0), (Color3f{1.f, 1.f, 0.f}));
    EXPECT_THAT(image->get_pixel(255,0), (Color3f{0.f, 1.f, 0.f}));
    EXPECT_THAT(image->get_pixel(0,255), (Color3f{1.f, 0.f, 0.f}));
    EXPECT_THAT(image->get_pixel(255,255), (Color3f{0.f, 0.f, 1.f}));
}

TEST(Image, load_image_png_as_Image4b) {
    auto image = load_image_png_as_Image4b("imaging/tests/data/bay.png");

    // left side of the image is blue:
    for(int y = 0; y < image->height(); ++y) {
        for(int x = 0; x < 15; ++x) {
            EXPECT_THAT(image->get_pixel(x,y), (Color4b{1, 102, 177, 255}));
        }
    }

    // the right side of the image is white:
    for(int y = 0; y < image->height(); ++y) {
        for(int x = 20; x < image->width(); ++x) {
            EXPECT_THAT(image->get_pixel(x,y), (Color4b{255, 255, 255, 255}));
        }
    }
}

TEST(Image, load_png_file) {
    auto image = load_image_png("imaging/tests/data/bay.png");

    // the right side of the image is white:
    for(int y = 0; y < image->height(); ++y) {
        for(int x = 20; x < image->width(); ++x) {
            EXPECT_THAT(image->get_pixel(x,y), (Color3f{1.f, 1.f, 1.f}));
        }
    }
}
