/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/exception.hpp"

#include "gmock/gmock.h"

using de_vertexwahn::Exception;

TEST(Exception, what) {
    Exception ex{"Error reading flatland.xml"};
    EXPECT_STREQ(ex.what(), "Error reading flatland.xml");
}
