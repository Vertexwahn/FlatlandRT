/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/scene/shape/shape.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

class DummyShape : public Shape2f {
public:
    DummyShape(const Transform44f &transform) : Shape2f(transform) {
    }

    [[nodiscard]]
    virtual AxisAlignedBoundingBox2f bounds() const override {
        throw std::runtime_error("Not implemented yet");
    }

    virtual bool intersect(const Ray2f &ray, MediumEvent2f &its) const override { return false; }

    virtual std::string
    convert_to_svg(const int svgCanvasWidth, const int svgCanvasHeight) const override { return ""; };

    Transform44f &getTransform() {
        return transform_;
    }
};

TEST(Shape, GivenADummyShape_WhenInitializedWithTranslation_ExpectTranslationTransform) {
    auto t = translate(Vector2f{10.f, 20.f});
    DummyShape s{t};

    EXPECT_EQ(s.getTransform().matrix()(0, 3), 10.f);
    EXPECT_EQ(s.getTransform().matrix()(1, 3), 20.f);

    EXPECT_THROW(s.bounds(), std::runtime_error);
    EXPECT_THAT(s.convert_to_svg(100, 100), "");
}
