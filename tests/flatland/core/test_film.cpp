/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/core/film.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(Film, Film_ctor) {
    // Arrange
    int width = 200;
    int height = 200;
    std::string filename = "rendering.svg";
    Film f{width, height, filename};

    EXPECT_THAT(f.getWidth(), width);
    EXPECT_THAT(f.getHeight(), height);
    EXPECT_THAT(f.getFilename(), filename);
    EXPECT_THAT(f.getSize(), Vector2i(width, height));
}
