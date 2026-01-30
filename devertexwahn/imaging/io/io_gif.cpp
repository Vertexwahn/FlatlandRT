/*
 *  SPDX-FileCopyrightText: Copyright 2026 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/io/io_gif.hpp"
#include "imaging/color_converter.hpp"

#include "gif_lib.h"

#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <vector>

DE_VERTEXWAHN_BEGIN_NAMESPACE

namespace {

// Helper function to find the closest color in a color map
int find_closest_color(const ColorMapObject* color_map, uint8_t r, uint8_t g, uint8_t b) {
    int best_index = 0;
    int best_distance = INT_MAX;

    for (int i = 0; i < color_map->ColorCount; ++i) {
        int dr = color_map->Colors[i].Red - r;
        int dg = color_map->Colors[i].Green - g;
        int db = color_map->Colors[i].Blue - b;
        int distance = dr * dr + dg * dg + db * db;

        if (distance < best_distance) {
            best_distance = distance;
            best_index = i;
        }
    }

    return best_index;
}

// Helper function to build a color map from an image
ColorMapObject* build_color_map(const Image4b& image) {
    // Create a color map with up to 256 colors
    constexpr int max_colors = 256;
    std::vector<GifColorType> colors;
    colors.reserve(max_colors);

    // Collect unique colors (up to 256)
    for (int y = 0; y < image.height() && colors.size() < max_colors; ++y) {
        for (int x = 0; x < image.width() && colors.size() < max_colors; ++x) {
            auto pixel = image.get_pixel(x, y);
            GifColorType color;
            color.Red = pixel.red();
            color.Green = pixel.green();
            color.Blue = pixel.blue();

            // Check if color already exists
            bool found = false;
            for (const auto& c : colors) {
                if (c.Red == color.Red && c.Green == color.Green && c.Blue == color.Blue) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                colors.push_back(color);
            }
        }
    }

    // Ensure we have at least 2 colors (GIF requirement)
    if (colors.empty()) {
        colors.push_back({0, 0, 0});
    }
    if (colors.size() == 1) {
        colors.push_back({255, 255, 255});
    }

    // Round up to nearest power of 2
    int color_count = 2;
    while (color_count < static_cast<int>(colors.size())) {
        color_count *= 2;
    }
    if (color_count > 256) {
        color_count = 256;
    }

    // Pad with black if needed
    while (colors.size() < static_cast<size_t>(color_count)) {
        colors.push_back({0, 0, 0});
    }

    ColorMapObject* color_map = GifMakeMapObject(color_count, colors.data());
    return color_map;
}

} // anonymous namespace

bool store_gif(const char* filename, const Image4b& image) {
    int error_code = 0;
    GifFileType* gif_file = EGifOpenFileName(filename, false, &error_code);

    if (gif_file == nullptr) {
        return false;
    }

    // Build color map from image
    ColorMapObject* color_map = build_color_map(image);
    if (color_map == nullptr) {
        EGifCloseFile(gif_file, &error_code);
        return false;
    }

    // Set screen description
    if (EGifPutScreenDesc(gif_file, image.width(), image.height(), 8, 0, color_map) == GIF_ERROR) {
        GifFreeMapObject(color_map);
        EGifCloseFile(gif_file, &error_code);
        return false;
    }

    // Set image description
    if (EGifPutImageDesc(gif_file, 0, 0, image.width(), image.height(), false, nullptr) == GIF_ERROR) {
        GifFreeMapObject(color_map);
        EGifCloseFile(gif_file, &error_code);
        return false;
    }

    // Write pixel data
    std::vector<GifByteType> line(image.width());
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            auto pixel = image.get_pixel(x, y);
            line[x] = static_cast<GifByteType>(
                find_closest_color(color_map, pixel.red(), pixel.green(), pixel.blue()));
        }

        if (EGifPutLine(gif_file, line.data(), image.width()) == GIF_ERROR) {
            GifFreeMapObject(color_map);
            EGifCloseFile(gif_file, &error_code);
            return false;
        }
    }

    GifFreeMapObject(color_map);

    if (EGifCloseFile(gif_file, &error_code) == GIF_ERROR) {
        return false;
    }

    return true;
}

bool store_gif(const char* filename, const Image3f& image) {
    // Convert Image3f to Image4b
    Image4b image_4b(image.width(), image.height());

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            auto pixel = image.get_pixel(x, y);
            auto converted = convertTo_<ColorRGBA4b>::apply(pixel);
            image_4b.set_pixel(x, y, converted);
        }
    }

    return store_gif(filename, image_4b);
}

Image3f load_image_gif(const char* filename) {
    if (!std::filesystem::exists(filename)) {
        throw std::runtime_error("GIF file does not exist: " + std::string(filename));
    }

    int error_code = 0;
    GifFileType* gif_file = DGifOpenFileName(filename, &error_code);

    if (gif_file == nullptr) {
        throw std::runtime_error("Failed to open GIF file: " + std::string(filename));
    }

    // Read all GIF data
    if (DGifSlurp(gif_file) == GIF_ERROR) {
        DGifCloseFile(gif_file, &error_code);
        throw std::runtime_error("Failed to read GIF file: " + std::string(filename));
    }

    // Get the first image
    if (gif_file->ImageCount == 0) {
        DGifCloseFile(gif_file, &error_code);
        throw std::runtime_error("GIF file contains no images: " + std::string(filename));
    }

    SavedImage* saved_image = &gif_file->SavedImages[0];
    GifImageDesc* image_desc = &saved_image->ImageDesc;

    int width = image_desc->Width;
    int height = image_desc->Height;

    // Get color map (use local if available, otherwise global)
    ColorMapObject* color_map = image_desc->ColorMap;
    if (color_map == nullptr) {
        color_map = gif_file->SColorMap;
    }

    if (color_map == nullptr) {
        DGifCloseFile(gif_file, &error_code);
        throw std::runtime_error("GIF file has no color map: " + std::string(filename));
    }

    // Create output image
    Image3f result(width, height);

    // Convert indexed pixels to RGB
    GifByteType* raster_bits = saved_image->RasterBits;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = raster_bits[y * width + x];

            if (index >= color_map->ColorCount) {
                index = 0; // Fallback to first color
            }

            GifColorType* color = &color_map->Colors[index];
            float r = color->Red / 255.0f;
            float g = color->Green / 255.0f;
            float b = color->Blue / 255.0f;

            result.set_pixel(x, y, ColorRGB3f{r, g, b});
        }
    }

    DGifCloseFile(gif_file, &error_code);

    return result;
}

DE_VERTEXWAHN_END_NAMESPACE
