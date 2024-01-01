/*
 *  SPDX-FileCopyrightText: Copyright 2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/color_converter.h"
#include "imaging/color.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(ColorConverter, convert_Color4f_to_Color4b) {
    Color4f color{1.f, 0.f, 0.f, 1.f};

    Color4b converted_color = ColorConverter::convertTo<Color4b>(color);

    EXPECT_THAT(converted_color, (Color4b{255, 0, 0, 255}));
}

TEST(ColorConverter, convert_Color4b_to_Color3f) {
    Color4b color{255, 0, 0, 255};

    Color3f converted_color = ColorConverter::convertTo<Color3f>(color);

    EXPECT_THAT(converted_color, (Color3f{1.f, 0, 0}));
}
