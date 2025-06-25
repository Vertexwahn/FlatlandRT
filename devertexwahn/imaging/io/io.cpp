/*
 *  SPDX-FileCopyrightText: Copyright 2022-2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/color_converter.hpp"
#include "imaging/io/io.hpp"
#include "imaging/io/io_jpeg.hpp"
#include "imaging/io/io_openexr.hpp"
#include "imaging/io/io_png.hpp"
#include "imaging/io/io_ppm.hpp"
#include "imaging/io/io_webp.hpp"

#include "math/util.hpp"

#include "boost/algorithm/string.hpp"

#include "fmt/core.h"
#include "fmt/format.h"

#include <filesystem>
#include <string_view>

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
            ColorRGB3b c = image->get_pixel(x, y);
            ColorRGBA4b converted_color = ColorConverter::convertTo<ColorRGBA4b>(c);
            converted_image->set_pixel(x,y,converted_color);
        }
    }

    return converted_image;
}

void store_image(std::string_view filename, ReferenceCounted<Image3b> image) {
    auto image4b = convert_to_Image4b(image.get());
    store_image(filename, image4b);
}

void store_image(std::string_view filename, const Image3b &image) {
    if (boost::ends_with(filename, ".png")) {
        store_png(filename.data(), image);
    } else {
        throw std::runtime_error("Invalid file extension");
    }
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

std::string extract_file_extension(std::string_view filename) {
    size_t dot_pos = filename.rfind('.');

    if (dot_pos == std::string::npos)
        return "";

    std::string extension = std::string(filename.substr(dot_pos));

    return extension;
}

Image3f load_image(std::string_view filename) {
    auto extension = extract_file_extension(filename);

    if(extension.empty()) {
        throw std::runtime_error(fmt::format("A filename (\"{}\") has been provided without an extension.", filename));
    }

    if(!std::filesystem::exists(filename)) {
        throw std::runtime_error(fmt::format("File \"{}\" does not exist.", filename));
    }

    if (boost::ends_with(filename, ".exr")) {
        Image3f image = load_image_openexr(filename);
        return image;
    }

    if(boost::ends_with(filename, ".jpg")) {
        Image3f image = load_image_jpeg(filename.data());
        return image;
    }

    if (boost::ends_with(filename, ".png")) {
        ReferenceCounted<Image3f> tmp_image = load_image_png(filename);

        Image3f image = *tmp_image.get();

        return image;
    }

    throw std::runtime_error(fmt::format("Unsupported file extension (\"{}\").", extract_file_extension(filename)));
}

// TODO: refactor this - make this available as "central service"
ReferenceCounted<Image3b> convert_to_Image3b(const Image4b* image) {
    auto converted_image = make_reference_counted<Image3b>(image->width(), image->height());

    for (int y = 0; y < image->height(); ++y) {
        for (int x = 0; x < image->width(); ++x) {
            ColorRGBA4b c = image->get_pixel(x, y);
            auto converted_color = ColorConverter::convertTo<ColorRGB3b>(c);
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

Image4f load_image_asImage4f(std::string_view filename) {
    if(std::filesystem::exists(filename) == false) {
        throw std::runtime_error(fmt::format("File \"{}\" does not exist.", filename));
    }

    if (boost::ends_with(filename, ".png")) {
        auto image = load_image_png_as_Image4b(filename);

        Image4f converted_image = Image4f(image->width(), image->height());

        for (int y = 0; y < image->height(); ++y) {
            for (int x = 0; x < image->width(); ++x) {
                ColorRGBA4b c = image->get_pixel(x, y);
                auto converted_color = ColorConverter::convertTo<ColorRGBA4f>(c);
                converted_image.set_pixel(x,y,converted_color);
            }
        }

        //Image4f image = load_image_openexr(filename);
        return converted_image;
    }

    throw std::runtime_error("Invalid file extension");
}

DE_VERTEXWAHN_END_NAMESPACE
