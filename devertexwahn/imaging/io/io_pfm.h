/*
 *  SPDX-FileCopyrightText: Copyright 2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_pfm_98f3ed1f_5b8d_49de_8ce2_c28374e1c292_h
#define De_Vertexwahn_Core_pfm_98f3ed1f_5b8d_49de_8ce2_c28374e1c292_h

#include "core/namespace.h"
#include "core/reference_counted.h"

#include "imaging/image.h"

#include <string_view>

DE_VERTEXWAHN_BEGIN_NAMESPACE

ReferenceCounted<Image3f> load_image_pfm(std::string_view filename);

bool store_pfm(std::string_view filename, const Image3f &image);

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Core_pfm_98f3ed1f_5b8d_49de_8ce2_c28374e1c292_h
