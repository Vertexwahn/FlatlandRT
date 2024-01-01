/*
 *  SPDX-FileCopyrightText: Copyright 2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "Eigen/Core"
#include "Eigen/Dense"

#include "gmock/gmock.h"

TEST(Eigen, Basic) {
    Eigen::Matrix<double, 5, 1, Eigen::DontAlign> test;

    test << 4, 3, 2, 1, 0;

    EXPECT_THAT(test[0], 4);
    EXPECT_THAT(sizeof(test), sizeof(double)*5);
}
