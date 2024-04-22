/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/bsdf/bsdf.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

class DummyBxDF : public BSDF2f {
public:
    DummyBxDF(const PropertySet &ps) : BSDF2f(ps) {}

    [[nodiscard]] Color sample(BSDFSample2f& sample, const Point2& sample_point) const override {
        return Color{0.f};
    }
    [[nodiscard]] Scalar pdf(const BSDFSample2f& sample) const override {
        return 0.f;
    }
    [[nodiscard]] Color evaluate(const BSDFSample2f& sample) const override {
        return Color{0.f};
    }
};

TEST(BxDF, WhenADefaultInitalized_ThenDefaultMaterialValues) {
    PropertySet ps;
    DummyBxDF bsdf{ps};

    BSDFSample2f bsdf_sample;
    bsdf_sample.wi = Vector2f{0.f, 1.f};
    bsdf_sample.wo = Vector2f{0.f, 1.f};
    MediumEvent2f me{};

    EXPECT_THAT(bsdf.sample(bsdf_sample, Point2f{0.f, 0.f}), Color3f(0.f));
    EXPECT_THAT(bsdf.evaluate(bsdf_sample), Color3f{0.f});
    //EXPECT_THAT(bsdf.refraction_index(), 1.f);
    //EXPECT_THAT(bsdf.interface_interaction_type(), InterfaceInteraction::SpecularTransmission);
}

TEST(BxDF, GivenAPropertySet_WhenInitializingMaterial_ExpectProperValues) {
    PropertySet ps;
    ps.add_property("refraction_index", 1.f);
    ps.add_property("reflection_index", 0.f);

    DummyBxDF bsdf(ps);
    //EXPECT_THAT(bsdf.refraction_index(), 1.f);
    //EXPECT_THAT(bsdf.interface_interaction_type(), InterfaceInteraction::SpecularTransmission);
}

TEST(BxDF, ctor_mirror) {
    PropertySet ps;
    ps.add_property("interface_interaction", std::string("mirror_reflection"));
    DummyBxDF bsdf(ps);

    //EXPECT_THAT(bsdf.interface_interaction_type(), InterfaceInteraction::MirrorReflection);
}

TEST(BxDF, ctor_undefined_type) {
    PropertySet ps;
    ps.add_property("interface_interaction", std::string("undefined_type"));
    DummyBxDF bsdf(ps);

    //EXPECT_THAT(bsdf.interface_interaction_type(), InterfaceInteraction::SpecularTransmission);
}
