/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/bxdf/svg_material.h"

#include "include/gmock/gmock.h"

using namespace de_vertexwahn;

TEST(SvgMaterial, SetGetStrokeColor) {
    PropertySet ps;
    auto stroke_color = Color3f{1.f, 0.5f, 0.1f};
    ps.add_property("stroke_color", stroke_color);
    SvgMaterial material{ps};
    EXPECT_THAT(material.stroke_color(), stroke_color);
}

TEST(SvgMaterial, SetGetfill_color) {
    auto fill_color = Color3f{1.f, 0.5f, 0.1f};
    PropertySet ps;
    ps.add_property("fill_color", fill_color);
    SvgMaterial material{ps};
    EXPECT_THAT(material.fill_color(), fill_color);
}

TEST(SvgMaterial, WhenADefaultInitalized_ThenDefaultMaterialValues) {
    PropertySet ps;
    SvgMaterial material{ps};
    EXPECT_THAT(material.stroke_color(), Color3f(0.f, 0.f, 0.f));
    EXPECT_THAT(material.fill_color(), Color3f(1.f, 1.f, 1.f));
    EXPECT_THAT(material.stroke_width(), 1.f);
}

TEST(SvgMaterial, GivenAPropertySet_WhenInitializingMaterial_ExpectProperValues) {
    PropertySet ps;
    ps.add_property("stroke_color", Color3f(1.f, 1.f, 1.f));
    ps.add_property("stroke_width", 3.f);
    ps.add_property("fill_color", Color3f(1.f, 1.f, 1.f));

    SvgMaterial material(ps);
    EXPECT_THAT(material.stroke_color(), Color3f(1.f, 1.f, 1.f));
    EXPECT_THAT(material.fill_color(), Color3f(1.f, 1.f, 1.f));
    EXPECT_THAT(material.stroke_width(), 3.f);
}
