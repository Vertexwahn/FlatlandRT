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

TEST(Image, round_trip_pfm) {
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