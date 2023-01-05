/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Imaging_io_openexr_a17ff2bc_adf7_432f_a1ec_3dbd77af587d_h
#define De_Vertexwahn_Imaging_io_openexr_a17ff2bc_adf7_432f_a1ec_3dbd77af587d_h

#include "core/reference_counted.h"
#include "imaging/image.h"

#include <string_view>

DE_VERTEXWAHN_BEGIN_NAMESPACE

Image3f load_image_openexr(std::string_view filename);

void store_open_exr(const std::string_view &filename, const Image3f &image);

void store_open_exr(const std::string_view &filename, ReferenceCounted<Image3f> image);

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Imaging_io_openexr_a17ff2bc_adf7_432f_a1ec_3dbd77af587d_h
