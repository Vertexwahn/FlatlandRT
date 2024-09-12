/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Imaging_io_ppm_a17ff2bc_adf7_432f_a1ec_3dbd77af587d_h
#define De_Vertexwahn_Imaging_io_ppm_a17ff2bc_adf7_432f_a1ec_3dbd77af587d_h

#include "imaging/image.hpp"

#include <string_view>

DE_VERTEXWAHN_BEGIN_NAMESPACE

void store_ppm(std::string_view filename, const Image4b &image);

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Imaging_io_ppm_a17ff2bc_adf7_432f_a1ec_3dbd77af587d_h
