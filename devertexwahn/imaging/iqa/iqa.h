/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Imaging_image_quality_assessment_a66e6fdf_0ace_4c78_b168_b24aa87d3475_h
#define De_Vertexwahn_Imaging_image_quality_assessment_a66e6fdf_0ace_4c78_b168_b24aa87d3475_h

#include "core/reference_counted.h"
#include "imaging/image.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

bool are_equal(const Image3f &a, const Image3f &b);
bool are_equal(const Image3b &a, const Image3b &b);

double mean_squared_error(const Image3f &a, const Image3f &b);
double mean_squared_error(ReferenceCounted<Image3f> a, ReferenceCounted<Image3f> b);

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Imaging_image_quality_assessment_a66e6fdf_0ace_4c78_b168_b24aa87d3475_h
