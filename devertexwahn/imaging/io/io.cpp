/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/color_converter.h"
#include "imaging/io/io.h"
#include "imaging/io/io_jpeg.h"
#include "imaging/io/io_openexr.h"
#include "imaging/io/io_png.h"
#include "imaging/io/io_ppm.h"
#include "imaging/io/io_webp.h"

#include "math/util.h"

#include "boost/algorithm/string.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

void store_image(std::string_view filename, ReferenceCounted<Image3f> image) {
    store_image(filename, *image.get());
}

void store_image(std::string_view filename, const Image3f &image) {
    if (boost::ends_with(filename, ".exr")) {
        store_open_exr(filename, image);
    } else if (boost::ends_with(filename, ".png")) {
        store_png(filename.data(), image);
    } else {
        throw std::runtime_error("Invalid file extension");
    }
}

// TODO: refactor this - make this available as "central service"
ReferenceCounted<Image4b> convert_to_Image4b(const Image3b* image) {
    auto converted_image = make_reference_counted<Image4b>(image->width(), image->height());

    for (int y = 0; y < image->height(); ++y) {
        for (int x = 0; x < image->width(); ++x) {
            Color3b c = image->get_pixel(x, y);
            Color4b converted_color = ColorConverter::convertTo<Color4b>(c);
            converted_image->set_pixel(x,y,converted_color);
        }
    }

    return converted_image;
}

void store_image(std::string_view filename, ReferenceCounted<Image3b> image) {
    auto image4b = convert_to_Image4b(image.get());
    store_image(filename, image4b);
}

void store_image(std::string_view filename, const Image4b &image) {
    if (boost::ends_with(filename, ".ppm")) {
        store_ppm(filename.data(), image);
    } else if (boost::ends_with(filename, ".png")) {
        store_png(filename.data(), image);
    } else if (boost::ends_with(filename, ".jpg")) {
        store_jpeg(filename.data(), image);
    } else if (boost::ends_with(filename, ".webp")) {
        store_webp(filename.data(), image);
    } else {
        throw std::runtime_error("Invalid file extension");
    }
}

void store_image(std::string_view filename, ReferenceCounted<Image4b> image) {
    store_image(filename, *image.get());
}

Image3f load_image(std::string_view filename) {
    if (boost::ends_with(filename, ".exr")) {
        Image3f image = load_image_openexr(filename);
        return image;
    }

    if (boost::ends_with(filename, ".png")) {
        ReferenceCounted<Image3f> tmp_image = load_image_png(filename);

        Image3f image = *tmp_image.get();

        return image;
    }

    throw std::runtime_error("Invalid file extension");
}

// TODO: refactor this - make this available as "central service"
ReferenceCounted<Image3b> convert_to_Image3b(const Image4b* image) {
    auto converted_image = make_reference_counted<Image3b>(image->width(), image->height());

    for (int y = 0; y < image->height(); ++y) {
        for (int x = 0; x < image->width(); ++x) {
            Color4b c = image->get_pixel(x, y);
            auto converted_color = ColorConverter::convertTo<Color3b>(c);
            converted_image->set_pixel(x,y,converted_color);
        }
    }

    return converted_image;
}

Image3b load_image_asImage3b(std::string_view filename) {
    if (boost::ends_with(filename, ".png")) {
        ReferenceCounted<Image4b> tmp_image = load_image_png_as_Image4b(filename);

        Image3b image = *convert_to_Image3b(tmp_image.get()).get();

        return image;
    }

    throw std::runtime_error("Invalid file extension");
}

DE_VERTEXWAHN_END_NAMESPACE
