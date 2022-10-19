/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/io/io_png.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

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
    EXPECT_THAT(loaded_image->get_pixel(1, 0), (Color3f{0.f, 1.f, 0.f}));
    EXPECT_THAT(loaded_image->get_pixel(0, 1), (Color3f{0.f, 0.f, 1.f}));
    //EXPECT_THAT(foo->get_pixel(1, 1), (Color3f{1.f, .5f, 0.f}));
}
