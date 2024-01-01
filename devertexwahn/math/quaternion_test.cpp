/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "math/quaternion.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(Point2f, Point2f_ctor) {
    Quaternionf q{0, 0, 3, 3};

    EXPECT_THAT(q.x(), 0.f);
}
