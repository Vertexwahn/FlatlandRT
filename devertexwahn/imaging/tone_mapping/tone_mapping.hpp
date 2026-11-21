/*
 *  SPDX-FileCopyrightText: Copyright 2025 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Imaging_ToneMapping_afd131db_a748_4dac_bf42_5078af3d7270_hpp
#define De_Vertexwahn_Imaging_ToneMapping_afd131db_a748_4dac_bf42_5078af3d7270_hpp

#include "imaging/image.hpp"

#include "core/namespace.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

Image3b srgb_operator(const Image3f& source_image);
Image4b srgb_operator(const Image4f& source_image);

// AgX tone mapping operator.
//
// Reproduces Blender's default "AgX" view transform (base look): a filmic
// display transform that gracefully rolls off HDR highlights instead of
// hard-clipping them (as a plain sRGB encode does), and desaturates very
// bright colours towards white.  Input is linear scene-referred (sRGB / Rec.709
// primaries, as produced by Cycles and Okapi); output is an 8-bit sRGB image.
//
// AgX consists of two fixed 3x3 matrices (inset/outset) around a per-channel
// log2 encode and a sigmoid ("contrast") curve.  The matrices here are the
// exact matrices from Blender's AgX configuration; the sigmoid is stored as a
// lookup table baked directly from the AgX view transform in Blender's
// OpenColorIO config, so the output matches Blender's AgX to within ~1.5 code
// values (as opposed to the common iolite polynomial approximation, which
// deviates by ~7 code values and visibly brightens the image).
Image3b agx_operator(const Image3f& source_image);

/*
Image4b reinhard(
    const Image4f& source_image,
    float log_average_luminance,
    float middle_key);
*/

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Imaging_ToneMapping_afd131db_a748_4dac_bf42_5078af3d7270_hpp
