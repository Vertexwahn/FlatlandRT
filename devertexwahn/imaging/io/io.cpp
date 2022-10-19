/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/io/io.h"
#include "imaging/io/io_jpeg.h"
#include "imaging/io/io_openexr.h"
#include "imaging/io/io_png.h"
#include "imaging/io/io_ppm.h"
#include "math/util.h"

#include "boost/algorithm/string.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

void store_image(std::string_view filename, ReferenceCounted<de_vertexwahn::Image3f> image) {
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

void store_image(std::string_view filename, const Image4b &image) {
    if (boost::ends_with(filename, ".ppm")) {
        store_image_ppm(filename.data(), image);
    } else if (boost::ends_with(filename, ".png")) {
        store_png(filename.data(), image);
    } else if (boost::ends_with(filename, ".jpg")) {
        store_jpeg(filename.data(), image);
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

DE_VERTEXWAHN_END_NAMESPACE