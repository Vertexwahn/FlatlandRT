/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/math/matrix.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(Matrix44f, Matrix44f_istream) {
    Matrix44f matrix;
    matrix << 1.0f, 2.0f, 3.0f, 4.0f,
              5.0f, 6.0f, 7.0f, 8.0f,
              9.0f, 10.0f, 11.0f, 12.0f,
              13.0f, 14.0f, 15.0f, 16.0f;

    EXPECT_THAT(matrix(0,0), 1.0f);
    EXPECT_THAT(matrix(0,1), 2.0f);
    EXPECT_THAT(matrix(0,2), 3.0f);
    EXPECT_THAT(matrix(0,3), 4.0f);
    EXPECT_THAT(matrix(3,3), 16.0f);
}

TEST(Matrix44f, Matrix44f_multiplication) {
    Matrix44f scaling;
    scaling << 3.0f, 0.0f, 0.0f, 0.0f,
               0.0f, 3.0f, 0.0f, 0.0f,
               0.0f, 0.0f, 3.0f, 0.0f,
               0.0f, 0.0f, 0.0f, 1.0f;

    Matrix44f translation;
    translation << 1.0f, 0.0f, 0.0f, 400.0f,
                   0.0f, 1.0f, 0.0f, 300.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f;

    Matrix44f result = translation * scaling;

    Matrix44f expectedResult;
    expectedResult << 3.0f, 0.0f, 0.0f, 400.0f,
                      0.0f, 3.0f, 0.0f, 300.0f,
                      0.0f, 0.0f, 3.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f;

    EXPECT_THAT(result, expectedResult);
}