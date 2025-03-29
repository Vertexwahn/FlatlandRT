/*
 *  SPDX-FileCopyrightText: Copyright 2022-2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/io/io.hpp"

#include "gmock/gmock.h"

#include <fstream>
#include <filesystem>

using namespace de_vertexwahn;

namespace de_vertexwahn {
    std::string extract_file_extension(std::string_view filename);
}

TEST(extract_file_extension, test) {
    EXPECT_THAT(extract_file_extension("image.exr"), std::string(".exr"));
    EXPECT_THAT(extract_file_extension("image.jepg"), std::string(".jepg"));
    EXPECT_THAT(extract_file_extension("image.jpg"), std::string(".jpg"));
    EXPECT_THAT(extract_file_extension("image.png"), std::string(".png"));
    EXPECT_THAT(extract_file_extension("image"), std::string(""));
    EXPECT_THAT(extract_file_extension("image.unsupported_extension"), std::string(".unsupported_extension"));
}

TEST(io, WhenTryToLoadANonExistingImage_ThenThrowException) {
    EXPECT_THAT([]() { load_image("not_exsiting_image.exr"); },
                ::testing::ThrowsMessage<std::runtime_error>("File \"not_exsiting_image.exr\" does not exist."));
}

TEST(io, WhenTryToLoadFileWithNotSupportedExtension_ThenThrowException) {
    // Arrange

    // write a dummy file
    std::ofstream file("image.unsupported_extension");
    EXPECT_TRUE(file.is_open());
    if(file.is_open()) {
        file << "Dummy file for testing load_image.\n";
        file.close();
    }

    // Act & Assert
    EXPECT_THAT([]() { load_image("image.unsupported_extension"); },
                ::testing::ThrowsMessage<std::runtime_error>("Unsupported file extension (\".unsupported_extension\")."));
}

TEST(io, WhenTryToLoadImageWithoutExtension_ThenThrowException) {
    // Arrange

    // write a dummy file without extension
    std::ofstream file("image");
    EXPECT_TRUE(file.is_open());
    if(file.is_open()) {
        file << "Dummy file for testing load_image.\n";
        file.close();
    }

    // Act & Assert
    EXPECT_THAT([]() { load_image("image"); },
        ::testing::ThrowsMessage<std::runtime_error>("A filename (\"image\") has been provided without an extension."));
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
            image.set_pixel(x, y, ColorRGB3f{1.f, 0.f, 0.f});
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
            ColorRGB3f c = image.get_pixel(x, y);
            ColorRGB3f ref_c = ref_image.get_pixel(x, y);
            EXPECT_THAT(ref_c, (ColorRGB3f{1.f, 0.f, 0.f}));
            EXPECT_THAT(ref_c, c);
        }
    }
}

TEST(Image3b, GivenTestImage3f_WhenStoreImageAsPng_ExpectCorectColorValues) {
    // Arrange
    auto image = make_reference_counted<Image3b>(100, 100);

    for (int y = 0; y < image->height(); ++y) {
        for (int x = 0; x < image->width(); ++x) {
            image->set_pixel(x, y, ColorRGB3b{255, 128, 0});
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
            ColorRGB3b c = image->get_pixel(x, y);
            ColorRGB3b ref_c = ref_image.get_pixel(x, y);
            EXPECT_THAT(ref_c, (ColorRGB3b{255, 128, 0}));
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

TEST(Image, WhenTryToLoadNonExistingFile_ThenThrowException) {
    EXPECT_THROW(load_image_asImage4f("non_existing_file.png"), std::runtime_error);
    EXPECT_THROW(load_image_asImage4f("non_existing_file.bmp"), std::runtime_error);
    EXPECT_THROW(load_image_asImage3b("non_existing_file.gif"), std::runtime_error);
}

TEST(Image, load_image_asImage4f) {
    // Arrange
    auto image = make_reference_counted<Image3b>(100, 100);

    for (int y = 0; y < image->height(); ++y) {
        for (int x = 0; x < image->width(); ++x) {
            image->set_pixel(x, y, ColorRGB3b{255, 128, 0});
        }
    }

    // Act
    store_image("test.png", image);

    auto img = load_image_asImage4f("test.png");
    EXPECT_THAT(img.get_pixel(0,0), (ColorRGBA4f{1.f, 0.501960814f, 0.f, 1.f}));
}
