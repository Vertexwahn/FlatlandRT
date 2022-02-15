/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Imaging_IO_1559b231_7e76_4e2f_9f7d_a24db5829202_h
#define Flatland_Imaging_IO_1559b231_7e76_4e2f_9f7d_a24db5829202_h

#include "flatland/imaging/image.h"
#include "flatland/core/reference_counted.h"

#include <string_view>

FLATLAND_BEGIN_NAMESPACE

void store_image(std::string_view filename, ReferenceCounted<Image3f> image);
void store_image(std::string_view filename, const Image3f& image);
void store_image(std::string_view filename, ReferenceCounted<Image4b> image);
void store_image(std::string_view filename, const Image4b& image);

Image3f load_image(std::string_view filename);

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Imaging_IO_1559b231_7e76_4e2f_9f7d_a24db5829202_h
