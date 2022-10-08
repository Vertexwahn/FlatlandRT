/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/io/io_png.h"
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

DE_VERTEXWAHN_BEGIN_NAMESPACE

ReferenceCounted<Image3f> load_image_png(std::string_view filename) {
    png_structp png_ptr;
    png_infop info_ptr;

    unsigned int sig_read = 0;
    FILE *fp = nullptr;
    if ((fp = fopen(filename.data(), "rb")) == nullptr)
        return nullptr;

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

    if (png_ptr == nullptr) {
        fclose(fp);
        return nullptr;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
        return nullptr;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
        fclose(fp);

        return nullptr;
    }

    png_init_io(png_ptr, fp);

    png_set_sig_bytes(png_ptr, sig_read);

    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND,
                 png_voidp_NULL);

    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);

    auto color_type = png_get_color_type(png_ptr, info_ptr);

    int color_count = 1;

    switch (color_type) {
        case PNG_COLOR_TYPE_RGBA:
            color_count = 4;
            break;
        case PNG_COLOR_TYPE_RGB:
            color_count = 3;
            break;
        case PNG_COLOR_TYPE_GRAY:
            color_count = 1;
            break;
        default:
            std::runtime_error("Color type not supported");
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            return nullptr;
    }

    typedef unsigned char byte;
    std::vector<byte> data;

    data.resize(width * height * color_count);

    ReferenceCounted<Image3f> image = make_reference_counted<Image3f>(width, height);

    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);

    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

    for (int y = 0; y < height; y++) {
        memcpy(&data[(row_bytes * (y))], row_pointers[y], row_bytes);

        for (int x = 0; x < width; x++) {
            float red = data[(height * color_count) * y + (x * color_count) + 0] / 255.0f;
            float green = data[(height * color_count) * y + (x * color_count) + 1] / 255.0f;
            float blue = data[(height * color_count) * y + (x * color_count) + 2] / 255.0f;

            image->set_pixel(x, y, Color3f(red, green, blue));
        }

    }

    return image;
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