/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "math/tensor.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(TensorXf, Scalar0DTensor) {
    TensorXf t{1.0f};
    //EXPECT_EQ(t.shape()[0], 0);
    //EXPECT_EQ(t.shape(0), 0);
    //EXPECT_EQ(t.values()[0], 1.0f);
}

/*
TEST(TensorXf, Matrix2DTensor) {
    TensorXf t{{1.0, 2.0, 3.0},
               {2.0, 5.0, 7.0},
               {2.0, 5.0, 7.0}};
}
*/
