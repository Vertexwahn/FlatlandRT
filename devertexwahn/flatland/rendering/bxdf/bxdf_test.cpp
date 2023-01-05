/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/bxdf/bxdf.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

class DummyBxDF : public BxDF2f {
public:
    DummyBxDF(const PropertySet &ps) : BxDF2f(ps) {}

    [[nodiscard]] Color3f evaluate(const Vector2f &wo, const Vector2f &wi) const override {
        return Color3f(0.f);
    }

    Vector2f sample(const Vector2f &wo, Vector2f& out_wi) const override {
        return Vector2f{0.f, 0.f};
    }
};

TEST(BxDF, WhenADefaultInitalized_ThenDefaultMaterialValues) {
    PropertySet ps;
    DummyBxDF bsdf{ps};

    EXPECT_THAT(bsdf.evaluate(Vector2f{1.f,0.f}, Vector2f{0.f,1.f} ), Color3f(0.f));
    Vector2f wi{1.f, 0.f};
    EXPECT_THAT(bsdf.sample(Vector2f{1.f,0.f}, wi), Vector2f(0.f,0.f));
    EXPECT_THAT(bsdf.refraction_index(), 1.f);
    EXPECT_THAT(bsdf.interface_interaction_type(), InterfaceInteraction::SpecularTransmission);
}

TEST(BxDF, GivenAPropertySet_WhenInitializingMaterial_ExpectProperValues) {
    PropertySet ps;
    ps.add_property("refraction_index", 1.f);
    ps.add_property("reflection_index", 0.f);

    DummyBxDF bsdf(ps);
    EXPECT_THAT(bsdf.refraction_index(), 1.f);
    EXPECT_THAT(bsdf.interface_interaction_type(), InterfaceInteraction::SpecularTransmission);
}

TEST(BxDF, ctor_mirror) {
    PropertySet ps;
    ps.add_property("interface_interaction", std::string("mirror_reflection"));
    DummyBxDF bsdf(ps);

    EXPECT_THAT(bsdf.interface_interaction_type(), InterfaceInteraction::MirrorReflection);
}

TEST(BxDF, ctor_undefined_type) {
    PropertySet ps;
    ps.add_property("interface_interaction", std::string("undefined_type"));
    DummyBxDF bsdf(ps);

    EXPECT_THAT(bsdf.interface_interaction_type(), InterfaceInteraction::SpecularTransmission);
}
