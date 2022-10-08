/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_Label2f_48266b1d_7018_4d01_b3fc_973b6e6bdf84_h
#define De_Vertexwahn_Flatland_Label2f_48266b1d_7018_4d01_b3fc_973b6e6bdf84_h

#include "math/point.h"

#include <string>

DE_VERTEXWAHN_BEGIN_NAMESPACE

template<typename ScalarType>
struct LabelType2 {
    Point2 <ScalarType> position;
    std::string text;
};

using Label2f = LabelType2<float>;
using Label2d = LabelType2<double>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Label2f_48266b1d_7018_4d01_b3fc_973b6e6bdf84_h
