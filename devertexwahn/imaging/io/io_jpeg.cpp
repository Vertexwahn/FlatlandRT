/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/io/io_jpeg.hpp"

#include "jpeglib.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

Image3f load_image_jpeg(const char* filename) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    FILE * infile;
    JSAMPARRAY buffer;
    int row_stride;

    if ((infile = fopen(filename, "rb")) == nullptr) {
        fprintf(stderr, "can't open %s\n", filename);
        exit(1);
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    (void) jpeg_read_header(&cinfo, TRUE);
    (void) jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)
            ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    Image3f image(cinfo.output_width, cinfo.output_height);

    while (cinfo.output_scanline < cinfo.output_height) {
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);
        for(int x = 0; x < cinfo.output_width; ++x) {
            image.set_pixel(x, cinfo.output_scanline - 1, ColorRGB3f(buffer[0][x * 3] / 255.f, buffer[0][x * 3 + 1] / 255.f, buffer[0][x * 3 + 2] / 255.f));
        }
    }

    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);

    return image;
}

bool store_jpeg(const char *filename, const Image4b &image) {
    int image_width = image.width();
    int image_height = image.height();
    int quality = 100;
    unsigned char *image_buffer = new unsigned char[image_width * image_height * 3];

    for(int x = 0; x < image_width; ++x) {
        for(int y = 0; y < image_height; ++y) {
            auto color = image.get_pixel(x,y);
            image_buffer[(y * image_width + x) * 3 + 0] = color.red();
            image_buffer[(y * image_width + x) * 3 + 1] = color.green();
            image_buffer[(y * image_width + x) * 3 + 2] = color.blue();
        }
    }

    int row_stride = sizeof(char8_t) * image_width * 3;

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    FILE * outfile;
    JSAMPROW row_pointer[1];

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    if ((outfile = fopen(filename, "wb")) == nullptr) {
        fprintf(stderr, "can't open %s\n", filename);
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

    return true;
}

bool store_jpeg(const char *filename, const Image3f &image) {
    int image_width = image.width();
    int image_height = image.height();
    int quality = 100;
    unsigned char *image_buffer = new unsigned char[image_width * image_height * 3];

    for(int x = 0; x < image_width; ++x) {
        for(int y = 0; y < image_height; ++y) {
            image_buffer[x * 3] = std::clamp(image.get_pixel(x, y).red() * 255, 0.f, 255.f);
            image_buffer[x * 3 + 1] = std::clamp(image.get_pixel(x, y).green() * 255, 0.f, 255.f);
            image_buffer[x * 3 + 2] = std::clamp(image.get_pixel(x, y).blue() * 255, 0.f, 255.f);
        }
    }

    //int row_stride = sizeof(char8_t) * image_width * 3;

    jpeg_compress_struct cinfo;
    jpeg_error_mgr jerr;

    FILE * outfile;
    JSAMPROW row_pointer[1];

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    if ((outfile = fopen(filename, "wb")) == nullptr) {
        fprintf(stderr, "can't open %s\n", filename);
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

    int row_stride = image_width * 3;

    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);

    delete [] image_buffer;

    return true;
}

DE_VERTEXWAHN_END_NAMESPACE
