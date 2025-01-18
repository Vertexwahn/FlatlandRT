/*
 *  SPDX-FileCopyrightText: Copyright 2022-2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/timer.hpp"

#include "gmock/gmock.h"

#include <chrono>
#include <thread>

using de_vertexwahn::Timer;
using namespace std::chrono_literals;

TEST(Timer, wait_for_one_second) {
    Timer timer;
    std::this_thread::sleep_for(1000ms);
    EXPECT_GE(timer.elapsed_seconds(), 1.0);
}

TEST(Timer, reset) {
    Timer timer;
    std::this_thread::sleep_for(1000ms);
    timer.reset();
    EXPECT_LE(timer.elapsed_seconds(), 1.0);
}

TEST(Timer, reset2) {
    Timer timer;
    std::this_thread::sleep_for(1000ms);
    timer.reset();
    EXPECT_LE(timer.elapsed_milliseconds(), 1000.0);
}
