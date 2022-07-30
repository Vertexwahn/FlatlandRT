/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/bsdf/bsdf.h"

#include "include/gmock/gmock.h"

using namespace flatland;

// Todo:
//bool mirrorReflection_ = false; -> enum class eInterfaceInteraction {specularTransmission, mirrorReflection}
// <string name="interface_interaction" value= "mirror_reflection"

TEST(Materal, WhenADefaultInitalized_ThenDefaultMaterialValues) {
    BSDF material;

    EXPECT_THAT(material.refraction_index(), 0.f);
    EXPECT_THAT(material.interface_interaction_type(), InterfaceInteraction::SpecularTransmission);
}

TEST(Material, GivenAPropertySet_WhenInitializingMaterial_ExpectProperValues) {
    PropertySet ps;
    ps.add_property("refraction_index", 1.f);
    ps.add_property("reflection_index", 0.f);

    BSDF material(ps);
    EXPECT_THAT(material.refraction_index(), 1.f);
    EXPECT_THAT(material.interface_interaction_type(), InterfaceInteraction::SpecularTransmission);
}

TEST(SvgMaterial, SetGetStrokeColor) {
    SvgMaterial material;
    auto stroke_color = Color3f(1.f, 0.5f, 0.1f);
    material.set_stroke_color(stroke_color);
    EXPECT_THAT(material.stroke_color(), stroke_color);
}

TEST(SvgMaterial, SetGetfill_color) {
    SvgMaterial material;
    auto fill_color = Color3f(1.f, 0.5f, 0.1f);
    material.set_fill_color(fill_color);
    EXPECT_THAT(material.fill_color(), fill_color);
}

TEST(SvgMaterial, WhenADefaultInitalized_ThenDefaultMaterialValues) {
    SvgMaterial material;

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
