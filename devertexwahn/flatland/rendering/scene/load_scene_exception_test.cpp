/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/scene/load_scene_exception.h"

#include "gmock/gmock.h"

using namespace flatland;

TEST(LoadSceneException, what) {
    LoadSceneException ex{"file.flatland.xml"};

    EXPECT_STREQ(ex.what(), "file.flatland.xml");
}
