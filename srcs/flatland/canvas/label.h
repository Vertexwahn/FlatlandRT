/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Label2f_48266b1d_7018_4d01_b3fc_973b6e6bdf84_h
#define Flatland_Label2f_48266b1d_7018_4d01_b3fc_973b6e6bdf84_h

#include "flatland/math/point.h"

#include <string>

FLATLAND_BEGIN_NAMESPACE

template <typename ScalarType>
struct LabelType2 {
	Point2<ScalarType> position;
	std::string text;
};

using Label2f = LabelType2<float>;
using Label2d = LabelType2<double>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Label2f_48266b1d_7018_4d01_b3fc_973b6e6bdf84_h
