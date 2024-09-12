/*
 *  SPDX-FileCopyrightText: Copyright 2022-2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/bsdf/svg_material.hpp"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(SvgMaterial, SetGetStrokeColor) {
    PropertySet ps;
    auto stroke_color = Color3f{1.f, 0.5f, 0.1f};
    ps.add_property("stroke_color", stroke_color);
    SvgMaterial material{ps};
    EXPECT_THAT(material.stroke_color(), stroke_color);
}

TEST(SvgMaterial, SetGetFillColor) {
    auto fill_color = Color3f{1.f, 0.5f, 0.1f};
    PropertySet ps;
    ps.add_property("fill_color", fill_color);
    SvgMaterial material{ps};
    EXPECT_THAT(material.fill_color(), fill_color);
}

TEST(SvgMaterial, BSDF) {
    PropertySet ps;
    auto stroke_color = Color3f{1.f, 0.5f, 0.1f};
    ps.add_property("stroke_color", stroke_color);
    SvgMaterial material{ps};

    BSDFSample2f bsdf_sample;
    bsdf_sample.wo = Vector2f{0.f, 1.f};
    bsdf_sample.wi = Vector2f{0.f, 1.f};
    MediumEvent2f me{};
    bsdf_sample.me = &me;

    EXPECT_THAT(material.sample(bsdf_sample, Vector2f{.5f}), Color3f{0.f});
    EXPECT_THAT(material.pdf(bsdf_sample), 0.f);
    EXPECT_THAT(material.evaluate(bsdf_sample), Color3f{0.f});
}

TEST(SvgMaterial, WhenADefaultInitalized_ThenDefaultMaterialValues) {
    PropertySet ps;
    SvgMaterial material{ps};
    EXPECT_THAT(material.stroke_color(), (Color3f{0.f, 0.f, 0.f}));
    EXPECT_THAT(material.fill_color(), (Color3f{1.f, 1.f, 1.f})); // TODO: Check if brackets can be moved to macro - maybe impossible because of C++ syntax?
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
