/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "fmt/core.h"

#include "include/gmock/gmock.h"

TEST(fmt, TextFormating) {
    std::string message = fmt::format("The answer is {}.", 42);

    EXPECT_THAT(message, "The answer is 42.");
}