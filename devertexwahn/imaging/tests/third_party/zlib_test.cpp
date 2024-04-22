/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "zlib.h"

#include "gmock/gmock.h"

TEST(zlib, version_check) {
    EXPECT_THAT(ZLIB_VERSION, testing::StrCaseEq("1.3.1"));
}
