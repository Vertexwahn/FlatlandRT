/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/imaging/io/io_openexr.h"

#include "include/gmock/gmock.h"

using namespace flatland;

TEST(Image, storeLoadOpenEXR) {
    // Arrange
    Image3f image{100, 100};

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            image.set_pixel(x, y, Color3f{1.f, 0.f, 0.f});
        }
    }

    // Act
    store_open_exr("test.exr", image);

    // Assert
    Image3f ref_image = load_image_openexr("test.exr");

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
