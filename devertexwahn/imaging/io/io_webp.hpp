/*
 *  SPDX-FileCopyrightText: Copyright 2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Imaging_io_webp_1ee82240_0345_4d89_916d_2757d731b9bd_h
#define De_Vertexwahn_Imaging_io_webp_1ee82240_0345_4d89_916d_2757d731b9bd_h

#include "imaging/image.hpp"

#include "core/reference_counted.hpp"

#include <string_view>

DE_VERTEXWAHN_BEGIN_NAMESPACE

bool store_webp(const char *filename, const Image4b &image);
ReferenceCounted<Image4b> load_image_webp(std::string_view filename);

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Imaging_io_webp_1ee82240_0345_4d89_916d_2757d731b9bd_h
