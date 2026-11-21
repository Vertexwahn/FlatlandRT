/*
 *  SPDX-FileCopyrightText: Copyright 2025 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/tone_mapping/tone_mapping.hpp"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

DE_VERTEXWAHN_BEGIN_NAMESPACE

std::uint8_t convert_to_uint8(float value_from_0_to_1);

DE_VERTEXWAHN_END_NAMESPACE

TEST(ToneMapping, convert_to_uint8) {
    EXPECT_THAT(convert_to_uint8(0.f), 0);
    EXPECT_THAT(convert_to_uint8(.5f), 128);
    EXPECT_THAT(convert_to_uint8(1.f), 255);
}

TEST(ToneMapping, srgb_operator) {
    Image4f image{1, 1};
    image.set_pixel(0, 0, ColorRGBA4f{1.0f, 0.f, 0.f, 1.f});
    Image4b result = srgb_operator(image);

    EXPECT_THAT(result.get_pixel(0,0).red(), 255);
    EXPECT_THAT(result.get_pixel(0,0).green(), 0);
}

TEST(ToneMapping, srgb_operator_on_image3f)
{
    Image3f image{1, 1};
    image.set_pixel(0, 0, ColorRGB3f{1.0f, 0.f, 0.f});
    Image3b result = srgb_operator(image);

    EXPECT_THAT(result.get_pixel(0,0).red(), 255);
    EXPECT_THAT(result.get_pixel(0,0).green(), 0);
}

// The expected values below are produced by Blender's AgX view transform
// (display "sRGB", view "AgX") and reproduced here by agx_operator to within
// one code value.
TEST(ToneMapping, agx_operator_maps_black_to_black) {
    Image3f image{1, 1};
    image.set_pixel(0, 0, ColorRGB3f{0.f, 0.f, 0.f});
    Image3b result = agx_operator(image);

    EXPECT_THAT(result.get_pixel(0, 0).red(), 0);
    EXPECT_THAT(result.get_pixel(0, 0).green(), 0);
    EXPECT_THAT(result.get_pixel(0, 0).blue(), 0);
}

TEST(ToneMapping, agx_operator_maps_middle_grey) {
    Image3f image{1, 1};
    image.set_pixel(0, 0, ColorRGB3f{0.18f, 0.18f, 0.18f});
    Image3b result = agx_operator(image);

    EXPECT_THAT(result.get_pixel(0, 0).red(), testing::AllOf(testing::Ge(117), testing::Le(119)));
    EXPECT_THAT(result.get_pixel(0, 0).green(), testing::AllOf(testing::Ge(117), testing::Le(119)));
    EXPECT_THAT(result.get_pixel(0, 0).blue(), testing::AllOf(testing::Ge(117), testing::Le(119)));
}

// AgX rolls off highlights instead of clipping them: a linear value of 1.0 does
// not become pure white and values well above 1.0 stay below 255.
TEST(ToneMapping, agx_operator_rolls_off_highlights) {
    Image3f image{1, 1};
    image.set_pixel(0, 0, ColorRGB3f{1.f, 1.f, 1.f});
    Image3b white = agx_operator(image);
    EXPECT_THAT(white.get_pixel(0, 0).red(), testing::AllOf(testing::Ge(195), testing::Le(197)));

    image.set_pixel(0, 0, ColorRGB3f{4.f, 4.f, 4.f});
    Image3b bright = agx_operator(image);
    EXPECT_THAT(bright.get_pixel(0, 0).red(), testing::AllOf(testing::Ge(232), testing::Le(234)));
    EXPECT_THAT(bright.get_pixel(0, 0).red(), testing::Lt(255));
}

// AgX desaturates highly saturated colors as they brighten.
TEST(ToneMapping, agx_operator_desaturates_pure_red) {
    Image3f image{1, 1};
    image.set_pixel(0, 0, ColorRGB3f{1.f, 0.f, 0.f});
    Image3b result = agx_operator(image);

    EXPECT_THAT(result.get_pixel(0, 0).red(), testing::AllOf(testing::Ge(204), testing::Le(206)));
    EXPECT_THAT(result.get_pixel(0, 0).green(), testing::AllOf(testing::Ge(23), testing::Le(25)));
    EXPECT_THAT(result.get_pixel(0, 0).blue(), testing::AllOf(testing::Ge(23), testing::Le(25)));
}

/*
TEST(ToneMapping, reinhard) {
    Image4f image{2, 2};

    float log_average_luminance = 2.f;
    float middle_key = .9f;
    Image4b result = reinhard(image, log_average_luminance, middle_key);
}
*/
