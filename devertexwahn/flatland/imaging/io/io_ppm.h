/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Imaging_io_ppm_a17ff2bc_adf7_432f_a1ec_3dbd77af587d_h
#define Flatland_Imaging_io_ppm_a17ff2bc_adf7_432f_a1ec_3dbd77af587d_h

#include "flatland/imaging/image.h"

#include <string_view>

FLATLAND_BEGIN_NAMESPACE

void store_image_ppm(std::string_view filename, const Image4b& image);

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Imaging_io_ppm_a17ff2bc_adf7_432f_a1ec_3dbd77af587d_h
