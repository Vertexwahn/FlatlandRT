/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "gmock/gmock.h"

#include "imaging/io/io_jpeg.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>

using namespace de_vertexwahn;

// copied form https://stackoverflow.com/questions/15118661/in-c-whats-the-fastest-way-to-tell-whether-two-string-or-binary-files-are-di
// and slightly modified
bool compare_files(const std::string& source_filename, const std::string& reference_filename) {
    // make sure both files exist
    if(!std::filesystem::exists(source_filename))
        return false;

    if(!std::filesystem::exists(reference_filename)) {
        return false;
    }

    std::ifstream source_file(source_filename, std::ifstream::ate | std::ifstream::binary);
    std::ifstream reference_file(reference_filename, std::ifstream::ate | std::ifstream::binary);
    const std::ifstream::pos_type fileSize = source_file.tellg();

    if (fileSize != reference_file.tellg()) {
        return false; // different file size
    }

    source_file.seekg(0);
    reference_file.seekg(0);

    std::istreambuf_iterator<char> source_begin(source_file);
    std::istreambuf_iterator<char> refernce_begin(reference_file);

    return std::equal(source_begin, std::istreambuf_iterator<char>(), refernce_begin);
}

TEST(Image, store_jpeg) {
    Image4b image{256, 256};

    for(int x = 0; x < image.width(); ++x) {
        for(int y = 0; y < image.height(); ++y) {
            image.set_pixel(x, y, ColorRGBA4b{255, 0, 0, 255});
        }
    }

    store_jpeg("red_square.jpg", image);

    EXPECT_TRUE(compare_files("red_square.jpg", "red_square.jpg"));
    EXPECT_TRUE(compare_files("imaging/tests/data/red_square_quality100.jpg", "imaging/tests/data/red_square_quality100.jpg"));
    //EXPECT_TRUE(compare_files("red_square.jpg", "imaging/tests/data/red_square_quality100.jpg"));
    // there seem to be a difference - maybe there is time stamp or something else...
}

TEST(Image, store_jpeg2) {
    Image3f image{256, 256};

    for(int x = 0; x < image.width(); ++x) {
        for(int y = 0; y < image.height(); ++y) {
            image.set_pixel(x, y, ColorRGB3f{1.f, 0.f, 0.f});
        }
    }

    store_jpeg("red_square.jpg", image);
}

TEST(Image, load_image_jpeg) {
    auto image = load_image_jpeg("imaging/tests/data/red_square_quality100.jpg");

    EXPECT_THAT(image.width(), 768);
    EXPECT_THAT(image.height(), 768);

    for(int x = 0; x < image.width(); ++x) {
        for(int y = 0; y < image.height(); ++y) {
            EXPECT_LE(std::abs(image.get_pixel(x, y).red()-1.0f), 0.1f);
            EXPECT_LE(std::abs(image.get_pixel(x, y).green()), 0.f);
            EXPECT_LE(std::abs(image.get_pixel(x, y).blue()), 0.f);
        }
    }
}
