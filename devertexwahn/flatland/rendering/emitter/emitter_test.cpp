/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/emitter/emitter.hpp"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

class DummyEmitter : public EmitterType<float, 2> {
public:
    DummyEmitter() : EmitterType<float, 2>() {}

    ColorTypeRGB<float, 3> evaluate() const override {
        return ColorTypeRGB<float, 3>(1.f);
    }
};

TEST(Emitter, to_string) {
    DummyEmitter emitter;
    EXPECT_THAT(emitter.to_string(), "Emitter");
    EXPECT_THAT(emitter.evaluate(), ColorRGB3f(1.f));
};
