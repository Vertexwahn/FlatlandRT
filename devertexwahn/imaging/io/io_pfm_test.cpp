/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/io/io_pfm.h"

#include "gmock/gmock.h"

#include <fstream>

using namespace de_vertexwahn;

TEST(store_pfm, expect_valid_header) {
    Image3f generated_image{2, 2};
    generated_image.set_pixel(0, 0, Color3f{1.f, 0.f, 0.f});
    generated_image.set_pixel(1, 0, Color3f{0.f, 1.f, 0.f});
    generated_image.set_pixel(0, 1, Color3f{0.f, 0.f, 1.f});
    generated_image.set_pixel(1, 1, Color3f{1.f, 1.f, 1.f});

    std::string filename = "test.pfm";

    store_pfm(filename, generated_image);

    std::ifstream stream(filename.c_str(), std::ios::binary);
    char header[2];
    stream.read(header, 2);

    EXPECT_THAT(header[0], 'P');
    EXPECT_THAT(header[1], 'F');
}

TEST(round_trip_pfm, one_pixel) {
    Image3f generated_image{1, 1};
    generated_image.set_pixel(0, 0, Color3f{1.f, 0.f, 0.f});
    store_pfm("test.pfm", generated_image);

    auto loaded_image = load_image_pfm("test.pfm");
    EXPECT_THAT(loaded_image->get_pixel(0, 0).red(), 1.f);
    EXPECT_THAT(loaded_image->get_pixel(0, 0).green(), 0.f);
    EXPECT_THAT(loaded_image->get_pixel(0, 0).blue(), 0.f);
}

TEST(round_trip_pfm, four_pixel) {
    Image3f generated_image{2, 2};

    EXPECT_THAT(generated_image.width(), 2);
    EXPECT_THAT(generated_image.height(), 2);

    generated_image.set_pixel(0, 0, Color3f{1.f, 0.f, 0.f});
    generated_image.set_pixel(1, 0, Color3f{0.f, 1.f, 0.f});
    generated_image.set_pixel(0, 1, Color3f{0.f, 0.f, 1.f});
    generated_image.set_pixel(1, 1, Color3f{1.f, 1.f, 1.f});

    store_pfm("test.pfm", generated_image);

    auto loaded_image = load_image_pfm("test.pfm");

    EXPECT_THAT(loaded_image->get_pixel(0, 0), (Color3f{1.f, 0.f, 0.f}));
    EXPECT_THAT(loaded_image->get_pixel(1, 0), (Color3f{0.f, 1.f, 0.f}));
    EXPECT_THAT(loaded_image->get_pixel(0, 1), (Color3f{0.f, 0.f, 1.f}));
    EXPECT_THAT(loaded_image->get_pixel(1, 1), (Color3f{1.f, 1.f, 1.f}));
}

TEST(load_image_pfm, given_PFM_with_no_content) {
    std::fstream file_with_no_content;
    file_with_no_content = std::fstream("test.pfm", std::ios::out | std::ios::binary);
    file_with_no_content.close();

    EXPECT_THAT([&]() { load_image_pfm("test.pfm"); },
                testing::ThrowsMessage<std::runtime_error>(testing::HasSubstr("Error reading first character of PFM file")));
}

TEST(load_image_pfm, given_PFM_with_only_one_char) {
    std::fstream file;
    file = std::fstream("test.pfm", std::ios::out | std::ios::binary);
    file << "P";
    file.close();

    EXPECT_THAT([&]() { load_image_pfm("test.pfm"); },
                testing::ThrowsMessage<std::runtime_error>(testing::HasSubstr("Error reading second character of PFM file")));
}

TEST(load_image_pfm, given_PFM_with_invalid_header) {
    std::fstream file;
    file = std::fstream("test.pfm", std::ios::out | std::ios::binary);
    file << "ABC";
    file.close();

    EXPECT_THAT([&]() { load_image_pfm("test.pfm"); },
                testing::ThrowsMessage<std::runtime_error>(testing::HasSubstr("First two characters for a valid PFM file must be 'PF' or 'Pf'.")));
}

TEST(load_image_pfm, given_PFM_with_only_two_valid_chars) {
    std::fstream file;
    file = std::fstream("test.pfm", std::ios::out | std::ios::binary);
    file << "PF";
    file.close();

    EXPECT_THAT([&]() { load_image_pfm("test.pfm"); },
                testing::ThrowsMessage<std::runtime_error>(testing::HasSubstr("Error reading third character of PFM file")));
}

TEST(load_image_pfm, given_PFM_with_third_char_is_not_a_whitespace) {
    std::fstream file;
    file = std::fstream("test.pfm", std::ios::out | std::ios::binary);
    file << "PFX";
    file.close();

    EXPECT_THAT([&]() { load_image_pfm("test.pfm"); },
                testing::ThrowsMessage<std::runtime_error>(testing::HasSubstr("The 3rd character of the input file is not whitespace.")));
}

TEST(load_image_pfm, given_PFM_EOF_after_file_identifier) {
    std::fstream file;
    file = std::fstream("test.pfm", std::ios::out | std::ios::binary);
    file << "PF ";
    file.close();

    EXPECT_THAT([&]() { load_image_pfm("test.pfm"); },
                testing::ThrowsMessage<std::runtime_error>(testing::HasSubstr("Error reading the width and height from input file.")));
}

TEST(load_image_pfm, given_PFM_invalid_width) {
    std::fstream file;
    file = std::fstream("test.pfm", std::ios::out | std::ios::binary);
    file << "PF 0 0 ";
    file.close();

    EXPECT_THAT([&]() { load_image_pfm("test.pfm"); },
                testing::ThrowsMessage<std::runtime_error>(testing::HasSubstr("Invalid input file: image width is zero")));
}

TEST(load_image_pfm, given_PFM_invalid_height) {
    std::fstream file;
    file = std::fstream("test.pfm", std::ios::out | std::ios::binary);
    file << "PF 1 0 ";
    file.close();

    EXPECT_THAT([&]() { load_image_pfm("test.pfm"); },
                testing::ThrowsMessage<std::runtime_error>(testing::HasSubstr("Invalid input file: image height is zero")));
}

TEST(load_image_pfm, given_PFM_invalid_scale_factor) {
    std::fstream file;
    file = std::fstream("test.pfm", std::ios::out | std::ios::binary);
    file << "PF 1 1 0";
    file.close();

    EXPECT_THAT([&]() { load_image_pfm("test.pfm"); },
                testing::ThrowsMessage<std::runtime_error>(testing::HasSubstr("Invalid input file format where scale factor is supposed to be")));
}

TEST(load_image_pfm, given_PFM_invalid_scale_factor_set_to_zero) {
    std::fstream file;
    file = std::fstream("test.pfm", std::ios::out | std::ios::binary);
    file << "PF" << std::endl;
    file << "1 1" << std::endl;
    file << "0.0" << std::endl;
    file.close();

    EXPECT_THAT([&]() { load_image_pfm("test.pfm"); },
                testing::ThrowsMessage<std::runtime_error>(testing::HasSubstr("Scale factor/endianness in PFM header is 0")));
}

TEST(load_image_pfm, given_PFM_invalid_size) {
    std::fstream file;
    file = std::fstream("test.pfm", std::ios::out | std::ios::binary);
    file << "PF" << std::endl;
    file << "a b" << std::endl;
    file << "0.0" << std::endl;
    file.close();

    EXPECT_THAT([&]() { load_image_pfm("test.pfm"); },
                testing::ThrowsMessage<std::runtime_error>(testing::HasSubstr("Invalid input file format where width and height are supposed to be ")));
}

TEST(load_image_pfm, given_PFM_invalid_size2) {
    std::fstream file;
    file = std::fstream("test.pfm", std::ios::out | std::ios::binary);
    file << "PF" << std::endl;
    file << "  8" << std::endl;
    file << "0.0" << std::endl;
    file.close();

    EXPECT_THAT([&]() { load_image_pfm("test.pfm"); },
                testing::ThrowsMessage<std::runtime_error>(testing::HasSubstr("Invalid input file format -- some character instead of white space")));
}

TEST(load_image_pfm, big_endian) {
    {
        std::ofstream file("test.pfm", std::ios::binary);
        file << "PF" << std::endl;
        file << 1 << " " << 1 << std::endl;
        file << "1.0" << std::endl;
        Color3f color{1.f, .5f, .1f};
        // this does not work - only on a big endian machine
        // todo: fix this in a way that bytes are written in big endian style
        file.write((char*) &color, sizeof(color));
    }

    auto loaded_image = load_image_pfm("test.pfm");

    // todo: fix this - get end
    //EXPECT_THAT(loaded_image->get_pixel(0,0).red(), 1.f);
    //EXPECT_THAT(loaded_image->get_pixel(0,0).green(), .5f);
    //EXPECT_THAT(loaded_image->get_pixel(0,0).blue(), .1f);
}
