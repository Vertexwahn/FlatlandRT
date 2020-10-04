/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Matrix_3b0aa566_7e67_433d_8fb6_075c84a4a70e_h
#define Flatland_Matrix_3b0aa566_7e67_433d_8fb6_075c84a4a70e_h

#include "flatland/core/namespace.h"

#include "Eigen/Core"
#include <Eigen/Dense>

FLATLAND_BEGIN_NAMESPACE

template <typename ScalarType>
using Matrix22 = Eigen::Matrix<ScalarType, 2, 2>;
template <typename ScalarType>
using Matrix33 = Eigen::Matrix<ScalarType, 3, 3>;
template <typename ScalarType>
using Matrix44 = Eigen::Matrix<ScalarType, 4, 4>;

using Matrix22f = Matrix22<float>;
using Matrix22d = Matrix22<double>;
using Matrix33f = Matrix33<float>;
using Matrix33d = Matrix33<double>;
using Matrix44f = Matrix44<float>;
using Matrix44d = Matrix44<double>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Matrix_3b0aa566_7e67_433d_8fb6_075c84a4a70e_h