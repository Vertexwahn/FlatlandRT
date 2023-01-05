/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Imaging_io_jpeg_f13dfab6_d49a_48d4_9c11_003af072c293_h
#define De_Vertexwahn_Imaging_io_jpeg_f13dfab6_d49a_48d4_9c11_003af072c293_h

#include "core/namespace.h"
#include "imaging/image.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

bool store_jpeg(const char *filename, const Image4b &image);

bool store_jpeg(const char *filename, const Image3f &image);

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Imaging_io_jpeg_f13dfab6_d49a_48d4_9c11_003af072c293_h