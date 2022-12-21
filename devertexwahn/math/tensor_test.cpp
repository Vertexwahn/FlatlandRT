// copy https://github.com/mitsuba-renderer/drjit/blob/77d2873a38d50a8d195bdcb3f2292f12987b0040/include/drjit/tensor.h#L187

/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
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
