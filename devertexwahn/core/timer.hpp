/*
 *  SPDX-FileCopyrightText: Copyright 2022-2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_Timer_28bff3a5_999c_4dc8_b1ea_c921f00f5f56_hpp
#define De_Vertexwahn_Core_Timer_28bff3a5_999c_4dc8_b1ea_c921f00f5f56_hpp

#include "core/namespace.hpp"

#include <chrono>

DE_VERTEXWAHN_BEGIN_NAMESPACE

class Timer {
public:
    Timer() noexcept;

    /*
     * @brief Elapsed seconds since timer object was created or reset via reset()
     */
    [[nodiscard]]
    double elapsed_seconds() const noexcept;

    /*
     * @brief Elapsed milliseconds since timer object was created or reset via reset()
     */
    [[nodiscard]]
    double elapsed_milliseconds() const noexcept;

    /*
     *  @brief Reset timer to 0
     */
    void reset() noexcept;

private:
    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;

    time_point last_; // last time when reset was called or the timer object was created
};

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Core_Timer_28bff3a5_999c_4dc8_b1ea_c921f00f5f56_hpp
