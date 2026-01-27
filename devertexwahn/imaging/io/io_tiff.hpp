/*
*  SPDX-FileCopyrightText: Copyright 2026 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_tiff_eb2face6_7c23_4b2f_bda9_925843b8e6af_hpp
#define De_Vertexwahn_Core_tiff_eb2face6_7c23_4b2f_bda9_925843b8e6af_hpp

#include "imaging/image.hpp"

#include "core/reference_counted.hpp"

#include <string_view>

DE_VERTEXWAHN_BEGIN_NAMESPACE

bool store_tiff(const char *filename, const Image4b &image);
ReferenceCounted<Image4b> load_image_tiff(std::string_view filename);

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Core_tiff_eb2face6_7c23_4b2f_bda9_925843b8e6af_hpp
