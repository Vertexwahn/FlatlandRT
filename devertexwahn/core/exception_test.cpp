/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/exception.h"

#include "include/gmock/gmock.h"

using namespace de_vertexwahn;

TEST(FlatlandException, what) {
    FlatlandException ex{"file.flatland.xml"};
    EXPECT_STREQ(ex.what(), "file.flatland.xml");
}
