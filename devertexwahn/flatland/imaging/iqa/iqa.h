/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Imaging_image_quality_assessment_a66e6fdf_0ace_4c78_b168_b24aa87d3475_h
#define Flatland_Imaging_image_quality_assessment_a66e6fdf_0ace_4c78_b168_b24aa87d3475_h

#include "flatland/core/reference_counted.h"
#include "flatland/imaging/image.h"

FLATLAND_BEGIN_NAMESPACE

bool are_equal(const Image3f &a, const Image3f &b);

double mean_squared_error(const Image3f &a, const Image3f &b);

double mean_squared_error(ReferenceCounted<Image3f> a, ReferenceCounted<Image3f> b);

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Imaging_image_quality_assessment_a66e6fdf_0ace_4c78_b168_b24aa87d3475_h