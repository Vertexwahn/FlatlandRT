/*
 *  SPDX-FileCopyrightText: Copyright 2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/object.h"

using namespace de_vertexwahn;

#include "gmock/gmock.h"

TEST(Object, to_string) {
    Object obj;
    EXPECT_THAT(obj.to_string(), "An object. Implement the to_string method to improve this description.");
}
