/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/core/timer.h"

FLATLAND_BEGIN_NAMESPACE

Timer::Timer() {
    start_ = clock::now();
}

void Timer::reset() {
    start_ = clock::now();
}

double Timer::elapsed_seconds() const {
    clock::time_point now = clock::now();
    int64_t elapsed_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now - start_).count();
    return elapsed_microseconds / 1000000.;
}

FLATLAND_END_NAMESPACE
