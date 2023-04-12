/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/exception.h"

#include "gmock/gmock.h"

using de_vertexwahn::Exception;

TEST(Exception, what) {
    Exception ex{"file.flatland.xml"};
    EXPECT_STREQ(ex.what(), "file.flatland.xml");
}
