/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "math/matrix.hpp"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

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

TEST(Matrix44f, scale_matrix) {
    Matrix44f result = scale_matrix(1.f, 2.f, 3.f);
    Matrix44f expected_result;
    expected_result << 1.f, 0.f, 0.f, 0.f,
                       0.f, 2.f, 0.f, 0.f,
                       0.f, 0.f, 3.f, 0.f,
                       0.f, 0.f, 0.f, 1.f;
    EXPECT_THAT(result, expected_result);
}

TEST(Matrix44f, translation_with_scalars) {
    Matrix44f result = translation_matrix(1.f, 2.f, 3.f);

    Matrix44f expected_result;
    expected_result << 1.f, 0.f, 0.f, 1.f,
            0.f, 1.f, 0.f, 2.f,
            0.f, 0.f, 1.f, 3.f,
            0.f, 0.f, 0.f, 1.f;
    EXPECT_THAT(result, expected_result);
}

TEST(Matrix44f, identity_matrix) {
    Matrix44f result = identity_matrix<float>();

    Matrix44f expected_result;
    expected_result << 1.f, 0.f, 0.f, 0.f,
                       0.f, 1.f, 0.f, 0.f,
                       0.f, 0.f, 1.f, 0.f,
                       0.f, 0.f, 0.f, 1.f;
    EXPECT_THAT(result, expected_result);
}
