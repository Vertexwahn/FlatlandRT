/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/io/io.h"

#include "gmock/gmock.h"

#include <fstream>
#include <filesystem>

using namespace de_vertexwahn;

TEST(io, WhenTryToLoadANonExistingImage_ThenThrowException) {
    EXPECT_THROW(load_image("muh.unknown_extension"), std::runtime_error);
}

TEST(io, WhenTryToStoreImageWithUnkownExtension_ThenThrowException) {
    Image4b img{100, 100};
    EXPECT_THROW(store_image("muh.unknown_extension", img), std::runtime_error);
}

TEST(Image, GivenTestImage3f_WhenStoreImageAsPng_ExpectCorectColorValues) {
    // Arrange
    Image3f image{100, 100};

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            image.set_pixel(x, y, Color3f{1.f, 0.f, 0.f});
        }
    }

    // Act
    store_image("test.png", image);

    // Assert
    Image3f ref_image = load_image("test.png");

    EXPECT_THAT(image.width(), ref_image.width());
    EXPECT_THAT(image.height(), ref_image.height());

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            Color3f c = image.get_pixel(x, y);
            Color3f ref_c = ref_image.get_pixel(x, y);
            EXPECT_THAT(ref_c, (Color3f{1.f, 0.f, 0.f}));
            EXPECT_THAT(ref_c, c);
        }
    }
}

TEST(Image3b, GivenTestImage3f_WhenStoreImageAsPng_ExpectCorectColorValues) {
    // Arrange
    auto image = make_reference_counted<Image3b>(100, 100);

    for (int y = 0; y < image->height(); ++y) {
        for (int x = 0; x < image->width(); ++x) {
            image->set_pixel(x, y, Color3b{255, 128, 0});
        }
    }

    // Act
    store_image("test.png", image);

    // Assert
    Image3b ref_image = load_image_asImage3b("test.png");

    EXPECT_THAT(image->width(), ref_image.width());
    EXPECT_THAT(image->height(), ref_image.height());

    for (int y = 0; y < image->height(); ++y) {
        for (int x = 0; x < image->width(); ++x) {
            Color3b c = image->get_pixel(x, y);
            Color3b ref_c = ref_image.get_pixel(x, y);
            EXPECT_THAT(ref_c, (Color3b{255, 128, 0}));
            EXPECT_THAT(ref_c, c);
        }
    }
}

TEST(Image, store_image_png) {
    ReferenceCounted<Image4b> img = make_reference_counted<Image4b>(2,2);
    store_image("test.png", img);
    EXPECT_TRUE(std::filesystem::exists("test.png"));
}

TEST(Image, store_image_ppm) {
    ReferenceCounted<Image4b> img = make_reference_counted<Image4b>(2,2);
    store_image("test.ppm", img);
    EXPECT_TRUE(std::filesystem::exists("test.ppm"));
}

TEST(Image, store_image_jpeg) {
    ReferenceCounted<Image4b> img = make_reference_counted<Image4b>(2,2);
    store_image("test.jpg", img);
    EXPECT_TRUE(std::filesystem::exists("test.jpg"));
}

TEST(Image, store_image_webp) {
    ReferenceCounted<Image4b> img = make_reference_counted<Image4b>(2,2);
    store_image("test.webp", img);
    EXPECT_TRUE(std::filesystem::exists("test.webp"));
}

TEST(Image, try_to_store_image4b_with_invalid_extension) {
    ReferenceCounted<Image4b> img = make_reference_counted<Image4b>(2,2);
    EXPECT_THROW(store_image("test.invalid_extension", img), std::runtime_error);
}

TEST(Image, try_to_store_image4f_with_invalid_extension) {
    Image3f img{2,2};
    EXPECT_THROW(store_image("test.invalid_extension", img), std::runtime_error);
}
