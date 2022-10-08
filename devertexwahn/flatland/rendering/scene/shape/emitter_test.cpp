/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/scene/shape/emitter.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(Emitter, to_string) {
    Emitter emitter;
    EXPECT_THAT(emitter.to_string(), "Emitter");
};
