/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xview.hpp"

#include <iostream>

#include "gmock/gmock.h"

typedef xt::xarray<float> TensorXf;

TEST(xtensor, Basic) {
    TensorXf storage;
    size_t height = 100;
    size_t width = 100;
    size_t color_channels = 3;
    storage.resize({height, width, color_channels});

    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            storage[{y,x,0}] = 1.f;
            storage[{y,x,1}] = 0.f;
            storage[{y,x,2}] = 0.f;
        }
    }

    EXPECT_THAT(storage[0], 1.f);
    EXPECT_THAT(storage[1], 0.f);
    EXPECT_THAT(storage[2], 0.f);
}

TEST(xtensor, Add) {
    // create a tensor with the shape {y=4,x=4,c=9}
    TensorXf tensor_;

    size_t height = 5;
    size_t width = 5;

    xt::xarray<float>::shape_type shape = {height, width, 9};
    tensor_ = xt::xarray<float>::from_shape(shape);

    // initialize tensor to zero
    for(int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < 9; ++c) {
                tensor_[{y, x, c}] = 0.f;
            }
        }
    }

    // increase tensor element y=2 x=3 c=3 by 4
    tensor_[{2, 3, 3}] += 4;

    // EXPECT that only y=2 x=3 c=3 by 4

    EXPECT_THAT((tensor_[{2, 3, 3}]), 4.f);

    for(int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < 9; ++c) {
                if(y == 2 && x == 3 && c == 3) {
                    EXPECT_THAT((tensor_[{y, x, c}]), 4.f);
                }
                else
                    EXPECT_THAT((tensor_[{y, x, c}]), 0.f);
            }
        }
    }

    EXPECT_THAT((tensor_[{0, 0, 0}]), 0.f);
    EXPECT_THAT((tensor_[{0, 1, 0}]), 0.f);
    EXPECT_THAT((tensor_[{0, 2, 0}]), 0.f);
    EXPECT_THAT((tensor_[{0, 3, 0}]), 0.f);
    EXPECT_THAT((tensor_[{0, 4, 0}]), 0.f);

    EXPECT_THAT((tensor_[{1, 0, 0}]), 0.f);
    EXPECT_THAT((tensor_[{1, 1, 0}]), 0.f);
    EXPECT_THAT((tensor_[{1, 2, 0}]), 0.f);
    EXPECT_THAT((tensor_[{1, 3, 0}]), 0.f);
    EXPECT_THAT((tensor_[{1, 4, 0}]), 0.f);

    EXPECT_THAT((tensor_[{2, 0, 0}]), 0.f);
    EXPECT_THAT((tensor_[{2, 1, 0}]), 0.f);
    EXPECT_THAT((tensor_[{2, 2, 0}]), 0.f);
    EXPECT_THAT((tensor_[{2, 3, 0}]), 0.f);
    EXPECT_THAT((tensor_[{2, 4, 0}]), 0.f);

    EXPECT_THAT((tensor_[{4, 0, 0}]), 0.f);
    EXPECT_THAT((tensor_[{4, 1, 0}]), 0.f);
    EXPECT_THAT((tensor_[{4, 2, 0}]), 0.f);
    EXPECT_THAT((tensor_[{4, 3, 0}]), 0.f);
    EXPECT_THAT((tensor_[{4, 4, 0}]), 0.f);
}
