/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/timer.h"

#include <stdint.h>

DE_VERTEXWAHN_BEGIN_NAMESPACE

Timer::Timer() {
    last_ = clock::now();
}

void Timer::reset() {
    last_ = clock::now();
}

double Timer::elapsed_seconds() const {
    clock::time_point now = clock::now();
    int64_t elapsed_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now - last_).count();
    return elapsed_microseconds / 1000000.;
}

double Timer::elapsed_milliseconds() const {
    clock::time_point now = clock::now();
    int64_t elapsed_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now - last_).count();
    return elapsed_microseconds / 1000.;
}


DE_VERTEXWAHN_END_NAMESPACE
