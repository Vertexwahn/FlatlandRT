/*
 *  SPDX-FileCopyrightText: Copyright 2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "fmt/core.h"

#include "gmock/gmock.h"

TEST(fmt, format) {
    std::string filename{"some_file.xyz"};
    std::stringstream ss;
    ss << "Loading file (" << filename <<  ") failed.";

    std::string s = fmt::format("Loading file ({}) failed.", filename);

    EXPECT_THAT(s, ss.str());
}
