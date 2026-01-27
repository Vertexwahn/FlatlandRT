/*
 *  SPDX-FileCopyrightText: Copyright 2026 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/io/io_tiff.hpp"

#include "tiffio.h"

#include <filesystem>

DE_VERTEXWAHN_BEGIN_NAMESPACE

bool store_tiff(const char *filename, const Image4b &image) {
    TIFF *tif = TIFFOpen(filename, "w");
    if (tif == nullptr) {
        return false;
    }

    int width = image.width();
    int height = image.height();

    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 4);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    uint16_t extra_samples[] = {EXTRASAMPLE_UNASSALPHA};
    TIFFSetField(tif, TIFFTAG_EXTRASAMPLES, 1, extra_samples);

    tsize_t linebytes = 4 * width;
    unsigned char *buf = nullptr;

    if (TIFFScanlineSize(tif) == linebytes) {
        buf = static_cast<unsigned char *>(_TIFFmalloc(linebytes));
    } else {
        buf = static_cast<unsigned char *>(_TIFFmalloc(TIFFScanlineSize(tif)));
    }

    if (buf == nullptr) {
        TIFFClose(tif);
        return false;
    }

    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(tif, width * 4));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            ColorRGBA4b color = image.get_pixel(x, y);
            buf[x * 4 + 0] = color.red();
            buf[x * 4 + 1] = color.green();
            buf[x * 4 + 2] = color.blue();
            buf[x * 4 + 3] = color.alpha();
        }
        if (TIFFWriteScanline(tif, buf, y, 0) < 0) {
            _TIFFfree(buf);
            TIFFClose(tif);
            return false;
        }
    }

    _TIFFfree(buf);
    TIFFClose(tif);
    return true;
}

ReferenceCounted<Image4b> load_image_tiff(std::string_view filename) {
    if (!std::filesystem::exists(filename)) {
        return nullptr;
    }

    TIFF *tif = TIFFOpen(filename.data(), "r");
    if (tif == nullptr) {
        return nullptr;
    }

    uint32_t width, height;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);

    uint32_t *raster = static_cast<uint32_t *>(_TIFFmalloc(width * height * sizeof(uint32_t)));
    if (raster == nullptr) {
        TIFFClose(tif);
        return nullptr;
    }

    if (!TIFFReadRGBAImageOriented(tif, width, height, raster, ORIENTATION_TOPLEFT, 0)) {
        _TIFFfree(raster);
        TIFFClose(tif);
        return nullptr;
    }

    auto image = make_reference_counted<Image4b>(width, height);

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            uint32_t pixel = raster[y * width + x];
            ColorRGBA4b color{
                static_cast<uint8_t>(TIFFGetR(pixel)),
                static_cast<uint8_t>(TIFFGetG(pixel)),
                static_cast<uint8_t>(TIFFGetB(pixel)),
                static_cast<uint8_t>(TIFFGetA(pixel))
            };
            image->set_pixel(x, y, color);
        }
    }

    _TIFFfree(raster);
    TIFFClose(tif);

    return image;
}

DE_VERTEXWAHN_END_NAMESPACE
