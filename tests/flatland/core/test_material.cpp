/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/core/material.h"

#include <gmock/gmock.h>

using namespace Flatland;

// Todo:
//bool mirrorReflection_ = false; -> enum class eInterfaceInteraction {specularTransmission, mirrorReflection}
// <string name="interface_interaction" value= "mirror_reflection"

TEST(Materal, WhenADefaultInitalized_ThenDefaultMaterialValues) {
    Material material;

    EXPECT_THAT(material.getStrokeColor(), Color3f(0.0f, 0.0f, 0.0f));
    EXPECT_THAT(material.getFillColor(), Color3f(1.0f, 1.0f, 1.0f));
    EXPECT_THAT(material.getStrokeWidth(), 1.0f);
    EXPECT_THAT(material.getRefractionIndex(), 0.0f);
    EXPECT_THAT(material.getInterInteractionType(), eInterfaceInteraction::specularTransmission);
}

TEST(Material, GivenAPropertySet_WhenInitializingMaterial_ExpectProperValues) {
    PropertySet ps;
    ps.addProperty("stroke_color", Color3f(1.0f, 1.0f, 1.0f));
    ps.addProperty("stroke_width", 3.0f);
    ps.addProperty("fill_color", Color3f(1.0f, 1.0f, 1.0f));
    ps.addProperty("refraction_index", 1.0f);
    ps.addProperty("reflection_index", 0.0f);

    Material material(ps);
    EXPECT_THAT(material.getStrokeColor(), Color3f(1.0f, 1.0f, 1.0f));
    EXPECT_THAT(material.getFillColor(), Color3f(1.0f, 1.0f, 1.0f));
    EXPECT_THAT(material.getStrokeWidth(), 3.0f);
    EXPECT_THAT(material.getRefractionIndex(), 1.0f);
    EXPECT_THAT(material.getInterInteractionType(), eInterfaceInteraction::specularTransmission);
}

TEST(Material, SetGetStrokeColor) {
    Material material;
    auto strokeColor = Color3f(1.0f, 0.5f, 0.1f);
    material.setStrokeColor(strokeColor);
    EXPECT_THAT(material.getStrokeColor(), strokeColor);
}

TEST(Material, SetGetFillColor) {
    Material material;
    auto fillColor = Color3f(1.0f, 0.5f, 0.1f);
    material.setFillColor(fillColor);
    EXPECT_THAT(material.getFillColor(), fillColor);
}
