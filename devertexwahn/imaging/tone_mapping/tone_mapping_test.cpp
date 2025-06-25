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
    Image4f image{2, 2};
    Image4b result = srgb_operator(image);
}

/*
TEST(ToneMapping, reinhard) {
    Image4f image{2, 2};

    float log_average_luminance = 2.f;
    float middle_key = .9f;
    Image4b result = reinhard(image, log_average_luminance, middle_key);
}
*/
