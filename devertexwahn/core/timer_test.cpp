/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/timer.h"

#include "include/gmock/gmock.h"

#include <iostream>
#include <chrono>
#include <thread>

using namespace de_vertexwahn;
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
