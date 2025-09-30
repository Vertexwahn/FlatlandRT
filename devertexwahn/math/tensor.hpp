/*
 *  SPDX-FileCopyrightText: Copyright 2022-2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_MainWindow_41629fd2_fe98_4bb5_a740_462c8b717333_hpp
#define De_Vertexwahn_Core_MainWindow_41629fd2_fe98_4bb5_a740_462c8b717333_hpp

#include "core/namespace.hpp"

#include <xtensor/containers/xarray.hpp>
#include <xtensor/io/xio.hpp>
#include <xtensor/views/xview.hpp>

#include <iostream>

DE_VERTEXWAHN_BEGIN_NAMESPACE

typedef xt::xarray<float> TensorXf;
typedef xt::xarray<double> TensorXd;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Core_MainWindow_41629fd2_fe98_4bb5_a740_462c8b717333_hpp
