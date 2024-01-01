/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Imaging_IO_1559b231_7e76_4e2f_9f7d_a24db5829202_h
#define De_Vertexwahn_Imaging_IO_1559b231_7e76_4e2f_9f7d_a24db5829202_h

#include "imaging/image.h"
#include "core/reference_counted.h"

#include <string_view>

DE_VERTEXWAHN_BEGIN_NAMESPACE

void store_image(std::string_view filename, ReferenceCounted<Image3f> image);
void store_image(std::string_view filename, ReferenceCounted<Image3b> image);
void store_image(std::string_view filename, ReferenceCounted<Image4b> image);

void store_image(std::string_view filename, const Image3f &image);
void store_image(std::string_view filename, const Image4b &image);

Image3f load_image(std::string_view filename);
Image3b load_image_asImage3b(std::string_view filename);

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Imaging_IO_1559b231_7e76_4e2f_9f7d_a24db5829202_h
