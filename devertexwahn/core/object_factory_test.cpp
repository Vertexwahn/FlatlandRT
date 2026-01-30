/*
 *  SPDX-FileCopyrightText: Copyright 2022-2026 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/integrator/integrator.hpp"
#include "flatland/rendering/shape/disk.hpp"

#include "core/object_factory.hpp"

#include "gmock/gmock.h"

#include <filesystem>

using namespace de_vertexwahn;
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

    ObjectFactory<PropertySet>::set_registration_callback( [](){ /* register nothing */ });
    auto& factory = ObjectFactory<PropertySet>::instance();

    factory.register_class<MockIntegrator>(integrator_name);

    PropertySet ps;
    ReferenceCounted<MockIntegrator> integrator =  std::static_pointer_cast<MockIntegrator>(factory.create_instance(integrator_name, ps));

    EXPECT_CALL(*integrator.get(), trace(_, _, _, _))
            .Times(1)
            .WillOnce(Return(ColorRGB3f(1.0f, 1.0f, 1.0f)));

    Ray2f ray{{0.f, 0.f}, {1.f, 0.f}, 0.f, 1.f};
    integrator->trace(nullptr, nullptr, ray, 1);
}

TEST(ObjectFactory, create_instance) {
    auto integrator_name = "mock_integrator";

    ObjectFactory<PropertySet>::set_registration_callback( [](){ /* register nothing */ });
    auto& factory = ObjectFactory<PropertySet>::instance();

    factory.register_class<MockIntegrator>(integrator_name);

    PropertySet ps;
    auto integrator = factory.create_instance(integrator_name, ps);

    EXPECT_TRUE(integrator.get());
}

TEST(ObjectFactory, WhenCreatingADisk_ThenDiskHasCorrectRadiusAndTransform) {
    // Arrange
    ObjectFactory<PropertySet>::set_registration_callback( [](){ /* register nothing */ });
    auto& factory = ObjectFactory<PropertySet>::instance();

    auto diskRadius = 5.f;

    PropertySet ps;
    ps.add_property("transform", translate(Vector2f{100.f, 200.f}));
    ps.add_property("radius", diskRadius);

    // Act
    factory.register_class<Disk2f>("Disk");
    auto shape = factory.create_instance("Disk", ps);

    ReferenceCounted<Disk2f> disk = std::dynamic_pointer_cast<Disk2f>(shape);

    // Assert
    EXPECT_THAT(disk->transform().matrix()(0, 3), 100.f);
    EXPECT_THAT(disk->transform().matrix()(1, 3), 200.f);
    EXPECT_THAT(disk->radius(), diskRadius);
}

TEST(ObjectFactory, WhenSameClassIsRegisteredTwice_ThenThrowException) {
    ObjectFactory<PropertySet>::set_registration_callback( [](){ /* register nothing */ });
    PropertySet ps;
    auto& factory = ObjectFactory<PropertySet>::instance();
    factory.register_class<Disk2f>("Disk");

    EXPECT_THROW(factory.register_class<Disk2f>("Disk"), ObjectFactoryClassAlreadyRegisteredException);
}

TEST(ObjectFactory, WhenTryingToCreateAnInstanceOfANotRegistedClass_ThenThrowException) {
    ObjectFactory<PropertySet>::set_registration_callback( [](){ /* register nothing */ });
    PropertySet ps;
    auto& factory = ObjectFactory<PropertySet>::instance();

    EXPECT_THROW(factory.create_instance("Muhkuh", ps), ObjectFactoryClassDoesNotExist);
}

TEST(ObjectFactory, WhenRegisterCallbackIsSet_ExpectAvailibiltyOfNewTypesAnUnavailbilityOfOldTypes) {
    // test property set for "Disk" (has transform and radius)
    PropertySet ps_disk;
    ps_disk.add_property("transform", translate(Vector2f{100.f, 200.f}));
    ps_disk.add_property("radius", 1.f);

    // test property set for "MockIntegrator" (empty be intention)
    PropertySet ps_mock_integrator;

    // start with an empty factory
    ObjectFactory<PropertySet>::set_registration_callback( [](){ /* register nothing */ });

    // nothing should be registered
    EXPECT_THROW(ObjectFactory<PropertySet>::instance().create_instance("Disk", ps_disk), ObjectFactoryClassDoesNotExist);
    EXPECT_THROW(ObjectFactory<PropertySet>::instance().create_instance("mock_integrator", ps_mock_integrator), ObjectFactoryClassDoesNotExist);

    // register "Disk"
    ObjectFactory<PropertySet>::set_registration_callback( []() {
        ObjectFactory<PropertySet>::instance().register_class<Disk2f>("Disk");
    });

    // This should call the register callback that registers Disk
    EXPECT_TRUE(ObjectFactory<PropertySet>::instance().create_instance("Disk", ps_disk) != nullptr);
    EXPECT_THROW(ObjectFactory<PropertySet>::instance().create_instance("mock_integrator", ps_mock_integrator), ObjectFactoryClassDoesNotExist);

    // register "mock_integrator"
    ObjectFactory<PropertySet>::set_registration_callback( []() {
        ObjectFactory<PropertySet>::instance().register_class<MockIntegrator>("mock_integrator");
    });

    EXPECT_THROW(ObjectFactory<PropertySet>::instance().create_instance("Disk", ps_disk), ObjectFactoryClassDoesNotExist);
    EXPECT_TRUE(ObjectFactory<PropertySet>::instance().create_instance("mock_integrator", ps_mock_integrator) != nullptr);
}
