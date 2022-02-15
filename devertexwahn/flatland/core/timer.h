/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Core_Timer_28bff3a5_999c_4dc8_b1ea_c921f00f5f56_h
#define Flatland_Core_Timer_28bff3a5_999c_4dc8_b1ea_c921f00f5f56_h

#include "flatland/core/namespace.h"

#include <chrono>

FLATLAND_BEGIN_NAMESPACE

class Timer {
public:
    Timer();

    void reset();

    [[nodiscard]]
    double elapsed_seconds() const;

private:
    using clock = std::chrono::steady_clock;
    clock::time_point start_;
};

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Core_Timer_28bff3a5_999c_4dc8_b1ea_c921f00f5f56_h