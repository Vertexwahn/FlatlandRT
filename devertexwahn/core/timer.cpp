/*
 *  SPDX-FileCopyrightText: Copyright 2022-2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/timer.hpp"

#include <cstdint>

DE_VERTEXWAHN_BEGIN_NAMESPACE

Timer::Timer() noexcept {
    last_ = clock::now();
}

double Timer::elapsed_seconds() const noexcept {
    clock::time_point now = clock::now();
    int64_t elapsed_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now - last_).count();
    return elapsed_microseconds / 1000000.;
}

double Timer::elapsed_milliseconds() const noexcept {
    clock::time_point now = clock::now();
    int64_t elapsed_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now - last_).count();
    return elapsed_microseconds / 1000.;
}

void Timer::reset() noexcept {
    last_ = clock::now();
}

DE_VERTEXWAHN_END_NAMESPACE
