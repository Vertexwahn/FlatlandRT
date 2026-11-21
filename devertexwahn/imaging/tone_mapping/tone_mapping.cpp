/*
 *  SPDX-FileCopyrightText: Copyright 2025 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/tone_mapping/tone_mapping.hpp"

#include <algorithm>
#include <array>
#include <cmath>

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

// AgX tone mapping — see tone_mapping.hpp for references.
namespace {

using Vec3 = std::array<float, 3>;

// Multiply a row-major 3x3 matrix by a column vector.
Vec3 mat3_mul(const std::array<Vec3, 3>& m, const Vec3& v) {
    return {
        m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2],
        m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2],
        m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2],
    };
}

// Blender's exact AgX sigmoid ("contrast") curve, baked from the AgX view
// transform in Blender's OpenColorIO config as a 65-entry lookup table over the
// normalized log2 domain [0, 1]. The stored values are display-linear (the sRGB
// OETF is applied separately afterwards). Using the exact curve instead of the
// common 6th-order polynomial approximation matches Blender's AgX output to
// within ~1.5 code values.
constexpr int kAgxSigmoidLutSize = 65;
constexpr std::array<float, kAgxSigmoidLutSize> kAgxSigmoidLut = {{
    0.00000000f, 0.00000061f, 0.00000334f, 0.00000943f,
    0.00002008f, 0.00003646f, 0.00006091f, 0.00009393f,
    0.00014016f, 0.00019993f, 0.00027804f, 0.00037881f,
    0.00050361f, 0.00066637f, 0.00086283f, 0.00111502f,
    0.00142255f, 0.00180185f, 0.00227719f, 0.00284107f,
    0.00356931f, 0.00443093f, 0.00551688f, 0.00684053f,
    0.00844617f, 0.01048019f, 0.01286671f, 0.01598404f,
    0.01968299f, 0.02432201f, 0.03005307f, 0.03695211f,
    0.04580050f, 0.05619911f, 0.06955082f, 0.08540453f,
    0.10477584f, 0.12812784f, 0.15531452f, 0.18701848f,
    0.22209948f, 0.25925989f, 0.29838294f, 0.33875409f,
    0.37916365f, 0.42096765f, 0.46074318f, 0.50137700f,
    0.54034953f, 0.57866438f, 0.61618255f, 0.65157452f,
    0.68691002f, 0.71936643f, 0.75148878f, 0.78180738f,
    0.81078983f, 0.83894133f, 0.86496515f, 0.89064778f,
    0.91429674f, 0.93732342f, 0.95915223f, 0.97929631f,
    0.99867088f,
}};

// Evaluate Blender's AgX sigmoid via the LUT with linear interpolation.
Vec3 agx_default_contrast(const Vec3& x) {
    Vec3 r{};
    for (int i = 0; i < 3; ++i) {
        const float pos = std::clamp(x[i], 0.0f, 1.0f) * (kAgxSigmoidLutSize - 1);
        const int idx = static_cast<int>(pos);
        const int next = std::min(idx + 1, kAgxSigmoidLutSize - 1);
        const float frac = pos - idx;
        r[i] = kAgxSigmoidLut[idx] * (1.0f - frac) + kAgxSigmoidLut[next] * frac;
    }
    return r;
}

// AgX input transform: inset matrix, log2 encoding and the sigmoid.
Vec3 agx(Vec3 val) {
    // Inset matrix (row-major; transposed from the column-major GLSL mat3).
    static const std::array<Vec3, 3> agx_mat = {{
        {0.842479062253094f, 0.0784335999999992f, 0.0792237451477643f},
        {0.0423282422610123f, 0.878468636469772f, 0.0791661274605434f},
        {0.0423756549057051f, 0.0784336f, 0.879142973793104f},
    }};

    constexpr float min_ev = -12.47393f;
    constexpr float max_ev = 4.026069f;

    val = mat3_mul(agx_mat, val);

    for (int i = 0; i < 3; ++i) {
        const float encoded = std::log2(std::max(val[i], 1e-10f));
        val[i] = (std::clamp(encoded, min_ev, max_ev) - min_ev) / (max_ev - min_ev);
    }

    return agx_default_contrast(val);
}

// AgX output transform (EOTF): outset matrix and decode to display-linear.
Vec3 agx_eotf(Vec3 val) {
    // Outset matrix (row-major; transposed from the column-major GLSL mat3).
    static const std::array<Vec3, 3> agx_mat_inv = {{
        {1.19687900512017f, -0.0980208811401368f, -0.0990297440797205f},
        {-0.0528968517574562f, 1.15190312990417f, -0.0989611768448433f},
        {-0.0529716355144438f, -0.0980434501171241f, 1.15107367264116f},
    }};

    val = mat3_mul(agx_mat_inv, val);

    // The sigmoid LUT already stores display-linear values, so the outset matrix
    // output only needs to be clamped to non-negative before the sRGB OETF is
    // applied afterwards (Blender's AgX targets an sRGB display).
    for (int i = 0; i < 3; ++i) {
        val[i] = std::max(val[i], 0.0f);
    }

    return val;
}

} // namespace

Image3b agx_operator(const Image3f& source_image) {
    Image3b out{source_image.width(), source_image.height()};

    for (int y = 0; y < source_image.height(); ++y) {
        for (int x = 0; x < source_image.width(); ++x) {
            ColorRGB3f color = source_image.get_pixel(x, y);

            Vec3 v{color.red(), color.green(), color.blue()};
            v = agx_eotf(agx(v));

            out.set_pixel(x, y,
                ColorRGB3b{
                    convert_to_uint8(to_srgb(v[0])),
                    convert_to_uint8(to_srgb(v[1])),
                    convert_to_uint8(to_srgb(v[2]))
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
