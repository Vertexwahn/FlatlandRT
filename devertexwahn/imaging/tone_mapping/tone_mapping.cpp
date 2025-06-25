/*
 *  SPDX-FileCopyrightText: Copyright 2025 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/tone_mapping/tone_mapping.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

float to_srgb(float value) {
    if (value < 0.0031308) {
        return 12.92 * value;
    }
    return 1.055 * pow(value, 0.41666) - 0.055;
}

// Maps a float value from the range [0.0, 1.0] to the integer range [0, 255].
// Values outside of this range get clamped
std::uint8_t convert_to_uint8(float value_from_0_to_1) {
    return static_cast<std::uint8_t>(std::clamp(std::round(value_from_0_to_1 * 255.f), 0.f, 255.f));
}

// Reference: https://github.com/tizian/tonemapper/blob/master/src/operators/SrgbOperator.cpp
Image3b srgb_operator(const Image3f& source_image) {
    Image3b out{source_image.width(), source_image.height()};

    for (int y = 0; y < source_image.height(); ++y) {
        for (int x = 0; x < source_image.width(); ++x) {
            ColorRGB3f color = source_image.get_pixel(x, y);

            out.set_pixel(x, y,
                ColorRGB3b{
                    convert_to_uint8(to_srgb(color.red())),
                    convert_to_uint8(to_srgb(color.green())),
                    convert_to_uint8(to_srgb(color.blue()))
                }
            );

        }
    }

    return out;
}

Image4b srgb_operator(const Image4f& source_image) {
    Image4b out{source_image.width(), source_image.height()};

    for (int y = 0; y < source_image.height(); ++y) {
        for (int x = 0; x < source_image.width(); ++x) {
            ColorRGBA4f color = source_image.get_pixel(x, y);

            out.set_pixel(x, y,
                ColorRGBA4b{
                    convert_to_uint8(to_srgb(color.red())),
                    convert_to_uint8(to_srgb(color.green())),
                    convert_to_uint8(to_srgb(color.blue())),
                    convert_to_uint8(to_srgb(color.alpha()))
                }
            );

        }
    }

    return out;
}

/*
Image4b reinhard(
    const Image4f& source_image,
    float log_average_luminance,
    float middle_key) {
    float luminance_max_value = 0.f;

    Image4b out{source_image.width(), source_image.height()};

    for (int y = 0; y < source_image.height(); ++y) {
        for (int x = 0; x < source_image.width(); ++x) {
            Color4f color = source_image.get_pixel(x, y);
            out.set_pixel(
                Color4b{toSRGB(color.red()),
                        toSRGB(color.green()),
                        toSRGB(color.blue()),
                        toSRGB(color.alpha())}
            );
        }
    }

    return out;
}
*/

DE_VERTEXWAHN_END_NAMESPACE
