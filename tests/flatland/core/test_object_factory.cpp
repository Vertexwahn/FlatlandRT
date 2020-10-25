/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/core/object_factory.h"
//#include "flatland/core/object_factory.h"
#include "flatland/integrator/integrator.h"
//#include "flatland/scene/scene.h"
#include "flatland/scene/shape/disk.h"

#include <gmock/gmock.h>

using namespace Flatland;

class MockIntegrator : public IntegratorType<2, float> {
public:
    MockIntegrator(const PropertySet& ps) : IntegratorType<2, float>(ps) {

    }

    virtual Color3f trace(
            const Scene *scene,
            Ray &ray,
            const int depth) const {
        return  Color3f(1.0f, 0.0f, 0.0f);
    }
};

// todo: transform it to a generic object factory
// instead of path name provide a generic context object
TEST(ObjectFactory, Test1) {
    auto integrator_name = "mock_integrator";

    ObjectFactory sf;
    sf.registerClass<MockIntegrator>(integrator_name);

    PropertySet ps;
    auto integrator = sf.createInstance(integrator_name, ps);

    EXPECT_TRUE(integrator.get());
}


TEST(ShapeFactory, WhenCreatingADisk_ThenDiskHasCorrectRadiusAndTransform) {
    // Arrange
    ObjectFactory sf;

    auto diskRadius = 5.0f;

    PropertySet ps;
    ps.addProperty("transform", translate(Vector2f{100.0f,200.0f}));
    ps.addProperty("radius", diskRadius);

    std::filesystem::path path;

    // Act
    sf.registerClass<Disk2f>("Disk");
    auto shape = sf.createInstance("Disk", ps);

    ReferenceCounted<Disk2f> disk = std::dynamic_pointer_cast<Disk2f>(shape);

    // Assert
    EXPECT_THAT( disk->getTransform().getMatrix()(0,3), 100.0f);
    EXPECT_THAT( disk->getTransform().getMatrix()(1,3), 200.0f);
    EXPECT_THAT( disk->getRadius(), diskRadius);
}

TEST(ShapeFactory, WhenSameClassIsRegisteredTwice_ThenThrowException) {
    ObjectFactory sf;
    sf.registerClass<Disk2f>("Disk");

    EXPECT_THROW(sf.registerClass<Disk2f>("Disk");, ObjectFactoryClassAlreadyRegisteredException);
}

TEST(ShapeFactory, WhenTryingToCreateAnInstanceOfANotRegistedClass_ThenThrowException) {
    ObjectFactory sf;
    PropertySet ps;
    std::filesystem::path path;
    EXPECT_THROW(sf.createInstance("Disk", ps), ObjectFactoryClassDoesNotExist);
}
