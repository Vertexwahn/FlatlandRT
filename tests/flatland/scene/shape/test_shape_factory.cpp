/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/scene/shape/disk.h"
#include "flatland/scene/scene_factory.h"
#include "flatland/scene/scene.h"

#include <gmock/gmock.h>

using namespace Flatland;

namespace Flatland {
    ReferenceCounted<Shape2f> createDisk(const PropertySet& ps, const fs::path& path);
}

TEST(ShapeFactory, WhenCreatingADisk_ThenDiskHasCorrectRadiusAndTransform) {
    // Arrange
    SceneFactory sf;

    auto diskRadius = 5.0f;

    PropertySet ps;
    ps.addProperty("transform", translate(Vector2f{100.0f,200.0f}));
    ps.addProperty("radius", diskRadius);

    fs::path path;

    // Act
    sf.registerClass("Disk", createDisk);
    auto shape = sf.createShape("Disk", ps, path);

    ReferenceCounted<Disk2f> disk = std::dynamic_pointer_cast<Disk2f>(shape);

    // Assert
    EXPECT_THAT( disk->getTransform().getMatrix()(0,3), 100.0f);
    EXPECT_THAT( disk->getTransform().getMatrix()(1,3), 200.0f);
    EXPECT_THAT( disk->getRadius(), diskRadius);
}

TEST(ShapeFactory, WhenSameClassIsRegisteredTwice_ThenThrowException) {
    SceneFactory sf;
    sf.registerClass("Disk", createDisk);

    EXPECT_THROW(sf.registerClass("Disk", createDisk), SceneFactoryClassAlreadyRegisteredException);
}

TEST(ShapeFactory, WhenTryingToCreateAnInstanceOfANotRegistedClass_ThenThrowException) {
    SceneFactory sf;
    PropertySet ps;
    fs::path path;
    EXPECT_THROW(sf.createShape("Disk", ps, path), SceneFactoryClassDoesNotExist);
}
