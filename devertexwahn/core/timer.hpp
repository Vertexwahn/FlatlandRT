/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_Timer_28bff3a5_999c_4dc8_b1ea_c921f00f5f56_h
#define De_Vertexwahn_Core_Timer_28bff3a5_999c_4dc8_b1ea_c921f00f5f56_h

#include "core/namespace.hpp"

#include <chrono>

DE_VERTEXWAHN_BEGIN_NAMESPACE

class Timer {
public:
    Timer();

    void reset();

    [[nodiscard]]
    double elapsed_seconds() const;

    [[nodiscard]]
    double elapsed_milliseconds() const;

private:
    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;

    time_point last_; // last time when reset was called or the timer object was created
};

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Core_Timer_28bff3a5_999c_4dc8_b1ea_c921f00f5f56_h
