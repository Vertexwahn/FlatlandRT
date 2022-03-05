/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/math/matrix.h"

#include "gmock/gmock.h"

using namespace flatland;

TEST(Matrix44f, Matrix44f_istream) {
    Matrix44f matrix;
    matrix << 1.f, 2.f, 3.f, 4.f,
              5.f, 6.f, 7.f, 8.f,
              9.f, 10.f, 11.f, 12.f,
              13.f, 14.f, 15.f, 16.f;

    EXPECT_THAT(matrix(0,0), 1.f);
    EXPECT_THAT(matrix(0,1), 2.f);
    EXPECT_THAT(matrix(0,2), 3.f);
    EXPECT_THAT(matrix(0,3), 4.f);
    EXPECT_THAT(matrix(3,3), 16.f);
}

TEST(Matrix44f, Matrix44f_multiplication) {
    Matrix44f scaling;
    scaling << 3.f, 0.f, 0.f, 0.f,
               0.f, 3.f, 0.f, 0.f,
               0.f, 0.f, 3.f, 0.f,
               0.f, 0.f, 0.f, 1.f;

    Matrix44f translation;
    translation << 1.f, 0.f, 0.f, 400.f,
                   0.f, 1.f, 0.f, 300.f,
                   0.f, 0.f, 1.f, 0.f,
                   0.f, 0.f, 0.f, 1.f;

    Matrix44f result = translation * scaling;

    Matrix44f expected_result;
    expected_result << 3.f, 0.f, 0.f, 400.f,
                       0.f, 3.f, 0.f, 300.f,
                       0.f, 0.f, 3.f, 0.f,
                       0.f, 0.f, 0.f, 1.f;

    EXPECT_THAT(result, expected_result);
}