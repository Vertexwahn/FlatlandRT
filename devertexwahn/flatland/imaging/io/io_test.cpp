/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/imaging/io/io.h"

#include "include/gmock/gmock.h"

#include <fstream>

using namespace flatland;

TEST(Image, WriteAndReadFile) {
    std::string filename = "file.txt";

    // Write file
    {
        std::ofstream file;
        file.open (filename);
        file << "Writing this to a file.";
        file.close();
    }

    std::string text = "test";

    // Read file
    {
        std::ifstream file(filename);
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        text = content;
    }

    EXPECT_THAT(text, "Writing this to a file.");
}

TEST(Image, GivenTestImage3f_WhenStoreImageAsPng_ExpectCorectColorValues) {
    // Arrange
    Image3f image{100, 100};

    for(int y = 0; y < image.height(); ++y) {
        for(int x = 0; x < image.width(); ++x) {
            image.set_pixel(x, y, Color3f{1.f, 0.f, 0.f});
        }
    }

    // Act
    store_image("test.png", image);

    // Assert
    Image3f ref_image = load_image("test.png");

    EXPECT_THAT(image.width(), ref_image.width());
    EXPECT_THAT(image.height(), ref_image.height());

    for(int y = 0; y < image.height(); ++y) {
        for(int x = 0; x < image.width(); ++x) {
            Color3f c = image.get_pixel(x, y);
            Color3f ref_c = ref_image.get_pixel(x, y);
            EXPECT_THAT(ref_c, (Color3f{1.f,0.f,0.f}));
            EXPECT_THAT(ref_c, c);
        }
    }
}
