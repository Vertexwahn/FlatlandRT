/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/bxdf/bxdf.h"

#include "include/gmock/gmock.h"

using namespace de_vertexwahn;

class DummyBSDF : public BxDF {
public:
    DummyBSDF(const PropertySet &ps) : BxDF(ps) {}

    [[nodiscard]] Color3f evaluate(const Vector3f &wo, const Vector3f &wi) const override {
        return Color3f(0.f);
    }

    Vector3f sample(const Vector3f &wo, Vector3f& out_wi) const override {
        return Vector3f{0.f, 0.f, 0.f};
    }
};

TEST(BxDF, WhenADefaultInitalized_ThenDefaultMaterialValues) {
    PropertySet ps;
    DummyBSDF bsdf{ps};

    EXPECT_THAT(bsdf.evaluate(Vector3f{1.f,0.f,0.f}, Vector3f{0.f,1.f,0.f} ), Color3f(0.f));
    Vector3f wi{1.f, 0.f, 0.f};
    EXPECT_THAT(bsdf.sample(Vector3f{1.f,0.f,0.f}, wi), Color3f(0.f));
    EXPECT_THAT(bsdf.refraction_index(), 1.f);
    EXPECT_THAT(bsdf.interface_interaction_type(), InterfaceInteraction::SpecularTransmission);
}

TEST(BxDF, GivenAPropertySet_WhenInitializingMaterial_ExpectProperValues) {
    PropertySet ps;
    ps.add_property("refraction_index", 1.f);
    ps.add_property("reflection_index", 0.f);

    DummyBSDF bsdf(ps);
    EXPECT_THAT(bsdf.refraction_index(), 1.f);
    EXPECT_THAT(bsdf.interface_interaction_type(), InterfaceInteraction::SpecularTransmission);
}
