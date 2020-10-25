/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/integrator/path_mirror_reflection.h"
#include "flatland/integrator/ambient_occlusion.h"
#include "flatland/scene/load_scene.h"

#include <pugixml.hpp>

#include <gmock/gmock.h>

#include <filesystem>

using namespace Flatland;

// Test Scene details
FLATLAND_BEGIN_NAMESPACE
    PropertySet readAllProperties(const pugi::xml_node &node);
    Transform44f readTransform(const pugi::xml_node &xmlTransform);
FLATLAND_END_NAMESPACE

TEST(loadScene, GivenFileWithPathMirrorReflection_WhenLoadingScene_ThenExpectValidPathMirrorReflectionIntegrator) {
    // Act
    auto scene = loadScene("scenes/mirror.flatland.xml");
    ASSERT_TRUE(scene);
    auto integrator = scene->getIntegrator();
    ASSERT_TRUE(integrator);

    // Assert
    ReferenceCounted<PathMirrorReflection2f> pst = std::dynamic_pointer_cast<PathMirrorReflection2f>(integrator);
    EXPECT_TRUE(pst.get());
}

TEST(loadScene, GivenFileWithAmbientOcclusionIntegrator_WhenLoadingScene_ThenExpectValidAmbientOcclusionIntegrator) {
    // Act
    auto scene = loadScene("scenes/ao.flatland.xml");
    ASSERT_TRUE(scene);
    auto integrator = scene->getIntegrator();
    ASSERT_TRUE(integrator);

    // Assert
    ReferenceCounted<AmbientOcclusion2f> ao = std::dynamic_pointer_cast<AmbientOcclusion2f>(integrator);
    EXPECT_TRUE(ao.get());
}

TEST(loadScene, ReadProperties) {
    // Arrange
    pugi::xml_document doc;
    auto result = doc.load_file("tests/scenes/property.test.flatland.xml");
    auto xmlShape = doc.child("scene").child("shape");

    // Act
    auto ps = readAllProperties(xmlShape);

    // Assert
    EXPECT_FLOAT_EQ(ps.getProperty<int>("int_property"), 42);
    EXPECT_FLOAT_EQ(ps.getProperty<float>("float_property"), 1.0f);
    EXPECT_THAT(ps.getProperty<bool>("bool_property"), true);
    EXPECT_THAT(ps.getProperty<std::string>("string_property"), "This is a string");
    EXPECT_EQ(ps.getProperty<Point2f>("point_property"), Point2f(3.0f, 4.0f));
    EXPECT_EQ(ps.getProperty<Vector2f>("vector_property"), Vector2f(1.0f, 2.0f));
    EXPECT_EQ(ps.getProperty<Color3f>("color_property"), Color3f(0.0f, 0.682f, 0.937f));
    EXPECT_TRUE(result);
}

TEST(loadScene, ReadTransform) {
    // Arrange
    pugi::xml_document doc;
    auto result = doc.load_file("scenes/disk.flatland.xml");
    auto xmlShape = doc.child("scene").child("shape");

    // Act
    auto transform = readTransform(xmlShape.child("transform"));

    // Assert
    EXPECT_THAT(transform.getMatrix()(0,3), 400.0f);
    EXPECT_TRUE(result);
}

TEST(loadScene, ReadCameraTransform) {
    // Arrange
    pugi::xml_document doc;
    auto result = doc.load_file("scenes/rectangle.flatland.xml");
    auto xmlCamera = doc.child("scene").child("camera");

    // Act
    auto transform = readTransform(xmlCamera.child("transform"));

    auto p = transform * Point2f{100.0f, 600.0f};
    EXPECT_NEAR(p.x(), 0.0f, 0.0001f);
    EXPECT_NEAR(p.y(), 0.0f, 0.0001f);

    // Assert
    //EXPECT_THAT(transform.getMatrix()(0,3), 100.0f);
    EXPECT_TRUE(result);
}

TEST(loadScene, ReadPolygonTransform) {
    // Arrange
    pugi::xml_document doc;
    auto result = doc.load_file("scenes/bunny.flatland.xml");
    auto xmlShape = doc.child("scene").child("shape");

    // Act
    auto transform = readTransform(xmlShape.child("transform"));

    Matrix44f scaling;
    scaling <<  3.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 3.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f;

    Matrix44f translation;
    translation <<  1.0f, 0.0f, 0.0f, 400.0f,
            0.0f, 1.0f, 0.0f, 300.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f;

    Matrix44f expectedResult = translation * scaling;

    // Assert
    EXPECT_THAT(transform.getMatrix(), expectedResult);
    EXPECT_TRUE(result);
}

TEST(loadScene, ReadMaterial) {
    // Arrange
    pugi::xml_document doc;
    auto result = doc.load_file("scenes/disk.flatland.xml");
    auto xmlShape = doc.child("scene").child("shape");

    // Act
    auto material = readAllProperties(xmlShape.child("material"));

    // Assert
    EXPECT_THAT(material.getProperty<Color3f>("stroke_color"), (Color3f{0.0, 0.682, 0.937}));
    EXPECT_THAT(material.getProperty<float>("stroke_width"), 3);
    EXPECT_THAT(material.getProperty<Color3f>("fill_color"), (Color3f{1.0f,1.0f,1.0f}));
    EXPECT_TRUE(result);
}