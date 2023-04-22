/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "core/namespace.h"

#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>

#include <iostream>

DE_VERTEXWAHN_BEGIN_NAMESPACE

typedef xt::xarray<float> TensorXf;
typedef xt::xarray<double> TensorXd;

DE_VERTEXWAHN_END_NAMESPACE
