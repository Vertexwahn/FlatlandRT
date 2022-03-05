/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/core/object_factory.h"
#include "flatland/rendering/integrator/integrator.h"
#include "flatland/rendering/scene/shape/disk.h"

#include "gmock/gmock.h"

using namespace flatland;

TEST(ObjectFactoryClassAlreadyRegisteredException, what) {
    ObjectFactoryClassAlreadyRegisteredException ofe{"houston we have a problem"};

    EXPECT_STREQ(ofe.what(), "Class with name 'houston we have a problem' was already registered");
}

class MockIntegrator : public IntegratorType<2, float> {
public:
    MockIntegrator(const PropertySet &ps) : IntegratorType<2, float>(ps) {

    }

    virtual Color3f trace(
            const Scene *scene,
            Sampler *sampler,
            Ray &ray,
            const int depth) const override {
        return Color3f{1.f, 0.f, 0.f};
    }
};

TEST(ObjectFactory, Test1) {
    auto integrator_name = "mock_integrator";

    ObjectFactory<PropertySet> sf;
    sf.register_class<MockIntegrator>(integrator_name);

    PropertySet ps;
    auto integrator = sf.create_instance(integrator_name, ps);

    EXPECT_TRUE(integrator.get());
}

TEST(ShapeFactory, WhenCreatingADisk_ThenDiskHasCorrectRadiusAndTransform) {
    // Arrange
    ObjectFactory<PropertySet> sf;

    auto diskRadius = 5.f;

    PropertySet ps;
    ps.add_property("transform", translate(Vector2f{100.f, 200.f}));
    ps.add_property("radius", diskRadius);

    std::filesystem::path path;

    // Act
    sf.register_class<Disk2f>("Disk");
    auto shape = sf.create_instance("Disk", ps);

    ReferenceCounted<Disk2f> disk = std::dynamic_pointer_cast<Disk2f>(shape);

    // Assert
    EXPECT_THAT(disk->transform().matrix()(0, 3), 100.f);
    EXPECT_THAT(disk->transform().matrix()(1, 3), 200.f);
    EXPECT_THAT(disk->radius(), diskRadius);
}

TEST(ShapeFactory, WhenSameClassIsRegisteredTwice_ThenThrowException) {
    ObjectFactory<PropertySet> sf;
    sf.register_class<Disk2f>("Disk");

    EXPECT_THROW(sf.register_class<Disk2f>("Disk"), ObjectFactoryClassAlreadyRegisteredException);

}

TEST(ShapeFactory, WhenTryingToCreateAnInstanceOfANotRegistedClass_ThenThrowException) {
    ObjectFactory<PropertySet> sf;
    PropertySet ps;

    EXPECT_THROW(sf.create_instance("Disk", ps), ObjectFactoryClassDoesNotExist);
}
