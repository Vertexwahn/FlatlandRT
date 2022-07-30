/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/core/exception.h"

#include "include/gmock/gmock.h"

using namespace flatland;

TEST(FlatlandException, what) {
    FlatlandException ex{"file.flatland.xml"};

    EXPECT_STREQ(ex.what(), "file.flatland.xml");
}
