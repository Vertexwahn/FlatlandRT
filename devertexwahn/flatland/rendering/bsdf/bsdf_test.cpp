/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/bsdf/bsdf.h"

#include "include/gmock/gmock.h"

using namespace flatland;

TEST(BSDF, WhenADefaultInitalized_ThenDefaultMaterialValues) {
    PropertySet ps;
    BSDF bsdf{ps};

    EXPECT_THAT(bsdf.refraction_index(), 1.f);
    EXPECT_THAT(bsdf.interface_interaction_type(), InterfaceInteraction::SpecularTransmission);
}

TEST(BSDF, GivenAPropertySet_WhenInitializingMaterial_ExpectProperValues) {
    PropertySet ps;
    ps.add_property("refraction_index", 1.f);
    ps.add_property("reflection_index", 0.f);

    BSDF bsdf(ps);
    EXPECT_THAT(bsdf.refraction_index(), 1.f);
    EXPECT_THAT(bsdf.interface_interaction_type(), InterfaceInteraction::SpecularTransmission);
}
