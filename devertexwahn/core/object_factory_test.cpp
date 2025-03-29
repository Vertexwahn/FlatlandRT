/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/object_factory.hpp"
#include "flatland/rendering/integrator/integrator.hpp"
#include "flatland/rendering/shape/disk.hpp"

using namespace de_vertexwahn;

#include <filesystem>

#include "gmock/gmock.h"

using namespace testing;

TEST(ObjectFactoryClassAlreadyRegisteredException, what) {
    ObjectFactoryClassAlreadyRegisteredException ofe{"houston we have a problem"};

    EXPECT_STREQ(ofe.what(), "Class with name 'houston we have a problem' was already registered");
}

class MockIntegrator : public IntegratorType<float, 2> {
public:
    explicit MockIntegrator(const PropertySet &ps) : IntegratorType<float, 2>(ps) {}

    MOCK_METHOD(ColorRGB3f, trace, (const Scene *scene, Sampler *sampler, Ray &ray, const int depth), (const, override));
};

// This test has only the job to increase the line test coverage to 100%
// since trace needs to be mock, put gets never called
TEST(MockIntegrator, trace) {
    auto integrator_name = "mock_integrator";

    ObjectFactory<PropertySet> sf;
    sf.register_class<MockIntegrator>(integrator_name);

    PropertySet ps;
    ReferenceCounted<MockIntegrator> integrator =  std::static_pointer_cast<MockIntegrator>(sf.create_instance(integrator_name, ps));

    EXPECT_CALL(*integrator.get(), trace(_, _, _, _))
            .Times(1)
            .WillOnce(Return(ColorRGB3f(1.0f, 1.0f, 1.0f)));

    Ray2f ray{{0.f, 0.f}, {1.f, 0.f}, 0.f, 1.f};
    integrator->trace(nullptr, nullptr, ray, 1);
}

TEST(ObjectFactory, create_instance) {
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
