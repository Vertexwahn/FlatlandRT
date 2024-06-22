/*
 *  SPDX-FileCopyrightText: Copyright 2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "gmock/gmock.h"

#include "spdlog/spdlog.h"

TEST(spdlog, test) {
    testing::internal::CaptureStdout();
    spdlog::info("Welcome to spdlog!");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_THAT(output, ::testing::HasSubstr("Welcome to spdlog!"));
}
