/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/io/io_png.h"
#include "imaging/color_converter.h"
#include "core/logging.h"

#include "png.h"

#ifndef png_voidp_NULL
#define png_voidp_NULL (png_voidp)nullptr
#endif

#ifndef png_infopp_NULL
#define png_infopp_NULL (png_infopp)nullptr
#endif

#ifndef int_p_NULL
#define int_p_NULL (int*)nullptr
#endif

#include <filesystem>

DE_VERTEXWAHN_BEGIN_NAMESPACE

/*
 * The following function contains code that is copied and modified from
 * https://gist.github.com/niw/5963798
 *
 * Here it the original copyright and licence note (all changes are copyright by Julian Amann
 * and fall under X11 license.)
 *
 * A simple libpng example program
 * http://zarb.org/~gc/html/libpng.html
 *
 * Modified by Yoshimasa Niwa to make it much simpler
 * and support all defined color_type.
 *
 * To build, use the next instruction on OS X.
 * $ brew install libpng
 * $ clang -lz -lpng16 libpng_test.c
 *
 * Copyright 2002-2010 Guillaume Cottenceau.
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */
ReferenceCounted<Image4b> load_image_png_as_Image4b(std::string_view filename) {
    int width, height;
    png_byte color_type;
    png_byte bit_depth;
    png_bytep *row_pointers = nullptr;

    FILE *fp = fopen(filename.data(), "rb");

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if(!png) abort();

    png_infop info = png_create_info_struct(png);
    if(!info) abort();

    if(setjmp(png_jmpbuf(png))) {
        throw std::runtime_error("Invalid file format");
        abort();
    }

    png_init_io(png, fp);

    png_read_info(png, info);

    width      = png_get_image_width(png, info);
    height     = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth  = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt

    if(bit_depth == 16)
        png_set_strip_16(png);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if(png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if(color_type == PNG_COLOR_TYPE_RGB ||
       color_type == PNG_COLOR_TYPE_GRAY ||
       color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if(color_type == PNG_COLOR_TYPE_GRAY ||
       color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    if (row_pointers) abort();

    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for(int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
    }

    png_read_image(png, row_pointers);

    fclose(fp);

    png_destroy_read_struct(&png, &info, nullptr);

    // copy image
    auto image = make_reference_counted<Image4b>(width, height);
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
                Color4b color{
                    row_pointers[y][x * 4 + 0],
                    row_pointers[y][x * 4 + 1],
                    row_pointers[y][x * 4 + 2],
                    row_pointers[y][x * 4 + 3]
                };
                image->set_pixel(x, y, color);
        }
    }

    // free memory
    for(int y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);

    return image;
}

ReferenceCounted<Image3f> convert_to_Image3f(const Image4b* image) {
    auto converted_image = make_reference_counted<Image3f>(image->width(), image->height());

    for (int y = 0; y < image->height(); ++y) {
        for (int x = 0; x < image->width(); ++x) {
            Color4b c = image->get_pixel(x, y);
            Color3f converted_color = ColorConverter::convertTo<Color3f>(c);
            converted_image->set_pixel(x,y,converted_color);
        }
    }

    return converted_image;
}

ReferenceCounted<Image4f> convert_to_Image4f(const Image4b* image) {
    auto converted_image = make_reference_counted<Image4f>(image->width(), image->height());

    for (int y = 0; y < image->height(); ++y) {
        for (int x = 0; x < image->width(); ++x) {
            Color4b c = image->get_pixel(x, y);
            Color4f converted_color = ColorConverter::convertTo<Color4f>(c);
            converted_image->set_pixel(x,y,converted_color);
        }
    }

    return converted_image;
}

ReferenceCounted<Image3f> load_image_png(std::string_view filename) {
    if(!std::filesystem::exists(filename.data())) {
        return nullptr;
    }
    try {
        auto image_4b = load_image_png_as_Image4b(filename);
        return convert_to_Image3f(image_4b.get());
    }
    catch(...) {
        return nullptr;
    }
}

ReferenceCounted<Image4f> load_image_png_as_Image4f(std::string_view filename) {
    if(!std::filesystem::exists(filename.data())) {
        return nullptr;
    }
    try {
        auto image_4b = load_image_png_as_Image4b(filename);
        return convert_to_Image4f(image_4b.get());
    }
    catch(...) {
        return nullptr;
    }
}

bool store_png(const char *filename, const Image4b &image) {
    int width = image.width();
    int height = image.height();

    FILE *fp = fopen(filename, "wb");
    if (fp == nullptr) {
        LOG(INFO) << "Store PNG: Could not open file " << filename;
        return false;
    }

    png_bytep row = nullptr;
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        LOG(ERROR) << "Store PNG: Could not allocate write struct (" << filename << ")";
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        LOG(ERROR) << "Store PNG: Could not allocate info struct (" << filename << ")";
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        LOG(ERROR) << "Store PNG: Set jumpbuf failed (" << filename << ")";
    }

    png_init_io(png_ptr, fp);

    png_set_IHDR(png_ptr, info_ptr, width, height,
                 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    row = (png_bytep) malloc(sizeof(png_byte) * width * 4); //I think the problems start here

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Color4b c = image.get_pixel(x, y).clamp(0, 255);
            row[x * 4 + 0] = c.red();
            row[x * 4 + 1] = c.green();
            row[x * 4 + 2] = c.blue();
            row[x * 4 + 3] = 255;
        }
        png_write_row(png_ptr, row); //I think this causes the segmentation fault
    }

    png_write_end(png_ptr, info_ptr);

    if (info_ptr != nullptr) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    if (row != nullptr) free(row);
    if (png_ptr != nullptr) png_destroy_write_struct(&png_ptr, (png_infopp) nullptr);
    if (fp != nullptr) fclose(fp);

    return true;
}

bool store_png(const char *filename, const Image3f &image) {
    int width = image.width();
    int height = image.height();

    FILE *fp = fopen(filename, "wb");
    if (fp == nullptr) {
        LOG(INFO) << "Store PNG: Could not open file " << filename;
        return false;
    }

    png_bytep row = nullptr;
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        LOG(ERROR) << "Store PNG: Could not allocate write struct (" << filename << ")";
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        LOG(ERROR) << "Store PNG: Could not allocate info struct (" << filename << ")";
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        LOG(ERROR) << "Store PNG: Set jumpbuf failed (" << filename << ")";
    }

    png_init_io(png_ptr, fp);

    png_set_IHDR(png_ptr, info_ptr, width, height,
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    row = (png_bytep) malloc(sizeof(png_byte) * width * 3); //I think the problems start here

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            row[x * 3] = std::clamp(image.get_pixel(x, y).red() * 255, 0.f, 255.f);
            row[x * 3 + 1] = std::clamp(image.get_pixel(x, y).green() * 255, 0.f, 255.f);
            row[x * 3 + 2] = std::clamp(image.get_pixel(x, y).blue() * 255, 0.f, 255.f);
        }
        png_write_row(png_ptr, row); //I think this causes the segmentation fault
    }

    png_write_end(png_ptr, info_ptr);

    if (info_ptr != nullptr) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    if (row != nullptr) free(row);
    if (png_ptr != nullptr) png_destroy_write_struct(&png_ptr, (png_infopp) nullptr);
    if (fp != nullptr) fclose(fp);

    return true;
}

DE_VERTEXWAHN_END_NAMESPACE
