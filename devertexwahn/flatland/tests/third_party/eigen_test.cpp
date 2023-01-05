/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "gmock/gmock.h"

#include "Eigen/Geometry"

TEST(Eigen, Eigen_Translate) {
    Eigen::Affine3f transform(Eigen::Translation3f(1,2,3));
    Eigen::Matrix4f matrix = transform.matrix();

    EXPECT_THAT(matrix(0,3), testing::FloatEq(1.f));
    EXPECT_THAT(matrix(1,3), testing::FloatEq(2.f));
}