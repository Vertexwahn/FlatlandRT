/*
 *  SPDX-FileCopyrightText: Copyright 2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/color_converter.hpp"
#include "imaging/color.hpp"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(ColorConverter, convert_Color4f_to_Color4b) {
    ColorRGBA4f color{1.f, 0.f, 0.f, 1.f};

    ColorRGBA4b converted_color = ColorConverter::convertTo<ColorRGBA4b>(color);

    EXPECT_THAT(converted_color, (ColorRGBA4b{255, 0, 0, 255}));
}

TEST(ColorConverter, convert_Color4b_to_Color3f) {
    ColorRGBA4b color{255, 0, 0, 255};

    ColorRGB3f converted_color = ColorConverter::convertTo<ColorRGB3f>(color);

    EXPECT_THAT(converted_color, (ColorRGB3f{1.f, 0, 0}));
}
