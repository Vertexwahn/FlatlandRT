/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "zlib.h"

#include "include/gmock/gmock.h"

TEST(Zlib, VersionCheck) {
    EXPECT_THAT(ZLIB_VERSION, testing::StrCaseEq("1.2.13"));
}
