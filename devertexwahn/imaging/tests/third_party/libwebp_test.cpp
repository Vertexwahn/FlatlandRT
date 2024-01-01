/*
 *  SPDX-FileCopyrightText: Copyright 2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "webp/encode.h"
#include "webp/decode.h"

#include "gmock/gmock.h"

TEST(libwebp, write_webp_test) {
    uint8_t rgb[3] = {255, 128, 0};

    uint8_t* out = nullptr;
    int stride = sizeof(uint8_t) * 3;
    auto size = WebPEncodeLosslessRGB(rgb, 1, 1, stride, &out);
    EXPECT_TRUE(size > 0);

    int width = 0;
    int height = 0;
    WebPGetInfo(out, size, &width, &height);

    EXPECT_THAT(width, 1);
    EXPECT_THAT(height, 1);
}
