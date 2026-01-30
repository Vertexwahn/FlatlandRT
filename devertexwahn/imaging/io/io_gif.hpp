/*
 *  SPDX-FileCopyrightText: Copyright 2026 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_gif_27ba957b_c23f_41d8_998a_8ddeb552a1c8_hpp
#define De_Vertexwahn_Core_gif_27ba957b_c23f_41d8_998a_8ddeb552a1c8_hpp

#include "core/namespace.hpp"
#include "imaging/image.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

bool store_gif(const char *filename, const Image4b &image);

bool store_gif(const char *filename, const Image3f &image);

Image3f load_image_gif(const char* filename);

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Core_gif_27ba957b_c23f_41d8_998a_8ddeb552a1c8_hpp
