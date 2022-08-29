/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "include/gmock/gmock.h"

#include "png.h"

#include <string>

// https://stackoverflow.com/questions/1821806/how-to-encode-png-to-buffer-using-libpng/1823604

void createPng(char *filename, int width, int height) {
    FILE *fp = fopen(filename, "wb");;
    png_bytep row = nullptr;
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    setjmp(png_jmpbuf(png_ptr));
    png_init_io(png_ptr, fp);
    png_set_IHDR(png_ptr, info_ptr, width, height,
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_text title_text;
    title_text.compression = PNG_TEXT_COMPRESSION_NONE;
    //title_text.key = "Title";
    //title_text.text = "My title";
    png_set_text(png_ptr, info_ptr, &title_text, 1);
    png_write_info(png_ptr, info_ptr);

    row = (png_bytep) malloc(sizeof(png_byte)*width*3); //I think the problems start here

    for(int y=0; y<height; y++) {
        for(int x=0; x<width; x++) {
            row[x*3] = 0; //Red
            row[x*3+1] = 0; //Green
            row[x*3+2] = 0; //Blue
        }
        png_write_row(png_ptr, row); //I think this causes the segmentation fault
    }

    png_write_end(png_ptr, info_ptr);
}

TEST(lippng, use_function) {

}

