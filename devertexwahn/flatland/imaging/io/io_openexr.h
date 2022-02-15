/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Imaging_io_openexr_a17ff2bc_adf7_432f_a1ec_3dbd77af587d_h
#define Flatland_Imaging_io_openexr_a17ff2bc_adf7_432f_a1ec_3dbd77af587d_h

#include "flatland/core/reference_counted.h"
#include "flatland/imaging/image.h"

#include <string_view>

FLATLAND_BEGIN_NAMESPACE

Image3f load_image_openexr(std::string_view filename);
void store_open_exr(const std::string_view &filename, const Image3f &image);
void store_open_exr(const std::string_view &filename, ReferenceCounted<Image3f> image);

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Imaging_io_openexr_a17ff2bc_adf7_432f_a1ec_3dbd77af587d_h
