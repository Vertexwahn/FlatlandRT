/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "png.h"

#include "gmock/gmock.h"

#ifndef png_voidp_NULL
#define png_voidp_NULL (png_voidp)nullptr
#endif

#ifndef png_infopp_NULL
#define png_infopp_NULL (png_infopp)nullptr
#endif

#ifndef int_p_NULL
#define int_p_NULL (int*)nullptr
#endif

TEST(libpng, load_png_test) {
    png_structp png_ptr;
    png_infop info_ptr;

    FILE *fp = nullptr;
    EXPECT_TRUE((fp = fopen("okapi/tests/images/IndependentSampler_spp_1_TentFilter_radius_1_slow.png", "rb")) != nullptr);

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

    EXPECT_TRUE(png_ptr != nullptr);
    
    info_ptr = png_create_info_struct(png_ptr);
    EXPECT_TRUE(info_ptr != nullptr);

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
        fclose(fp);

        ASSERT_TRUE(false);
    }
    else {
        png_init_io(png_ptr, fp);

        unsigned int sig_read = 0;
        png_set_sig_bytes(png_ptr, sig_read);

        png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, png_voidp_NULL);

        int width = png_get_image_width(png_ptr, info_ptr);
        int height = png_get_image_height(png_ptr, info_ptr);

        EXPECT_THAT(width, 400);
        EXPECT_THAT(height, 400);

        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
    }
}
