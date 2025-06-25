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

/*
Image4b reinhard(
    const Image4f& source_image,
    float log_average_luminance,
    float middle_key);
*/

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Imaging_ToneMapping_afd131db_a748_4dac_bf42_5078af3d7270_hpp
