/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "gmock/gmock.h"

#include "jpeglib.h"
#include <setjmp.h>
#include <string>

TEST(jpeglib, use_function) {
    std::string filename = "test.jpg";
    int image_width = 256;
    int image_height = 256;
    int quality = 100;
    unsigned char *image_buffer = new unsigned char[image_width * image_height * 3];

    for(int x = 0; x < image_width; ++x) {
        for(int y = 0; y < image_height; ++y) {
            image_buffer[(y * image_width + x) * 3 + 0] = 255;
            image_buffer[(y * image_width + x) * 3 + 1] = 0;
            image_buffer[(y * image_width + x) * 3 + 2] = 0;
        }
    }

    int row_stride = sizeof(char8_t) * image_width * 3;

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    FILE * outfile;
    JSAMPROW row_pointer[1];

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    if ((outfile = fopen(filename.c_str(), "wb")) == nullptr) {
        fprintf(stderr, "can't open %s\n", filename.c_str());
        exit(1);
    }
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = image_width;
    cinfo.image_height = image_height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    row_stride = image_width * 3;

    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);

    delete [] image_buffer;
}
