/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/scene/load_scene.h"
#include "flatland/core/property_set.h"
#include "flatland/core/reference_counted.h"
#include "flatland/math/util.h"
#include "flatland/scene/shape/disk.h"
#include "flatland/scene/shape/rectangle.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(Scene, GivenAScene_WhenAddingAAnnotations_ThenValidAnnotion) {
    // Arrange
    Scene2f scene;
    float x = 100.0f;
    float y = 200.0f;
    std::string text = "Text";

    // Act
    scene.addAnnotation(Label2f{{x, y}, text});

    // Assert
    EXPECT_THAT(scene.getAnnotationCount(), 1u);
    EXPECT_THAT(scene.getAnnotations()[0].position.x(), testing::FloatEq(x));
    EXPECT_THAT(scene.getAnnotations()[0].position.y(), testing::FloatEq(y));
    EXPECT_THAT(scene.getAnnotations()[0].text, text);
}

TEST(Scene, GivenSceneWithMultipleShapes_WhenRayHitsAllShapes_ThenClosesHit) {
    // Arrange
    auto scene = loadScene("tests/scenes/disks.flatland.xml");
    auto ray = scene->getCamera()->generateRay();

    // Act
    MediumEvent2f me;
    bool hit = scene->intersect(ray, me);

    // Assert
    EXPECT_TRUE(hit);
    EXPECT_THAT(me.p.x(), 100.0f);
    EXPECT_THAT(me.p.y(), 300.0f);
}

TEST(Scence, GivenRayThatHitsASceneWithDisk_ThenIntersectedDisk) {
    // Arrange
    auto transform = translate(Vector2f{100.0f, 100.0f});
    auto disk = makeReferenceCounted<Disk2f>(transform, 25.0f);

    auto scene = makeReferenceCounted<Scene2f>();
    scene->addShape(disk);

    Point2f rayOrigin{0.0f, 100.0f};
    Vector2f rayDirection{1.0f, 0.0f};
    rayDirection.normalize();
    Ray2f ray(rayOrigin, rayDirection, 0.0f, 10000.0f);

    // Act
    MediumEvent2f its;
    bool hit = scene->intersect(ray, its);

    // Assert
    EXPECT_TRUE(hit);
    EXPECT_THAT(its.p, (Point2f{75.0f, 100.0f}));
}

TEST(Scene, WhenLoadingASceneWithARectangle_ThenSceneContainsARectangle) {
    // Arrange
    auto scene = loadScene("scenes/rectangle.flatland.xml");

    EXPECT_NE(scene, nullptr);
    EXPECT_THAT(scene->getShapeCount(), 1u);
    auto rect = std::dynamic_pointer_cast<Rectangle2f>(scene->getShapes()[0]);
    EXPECT_NE(rect, nullptr);
}

TEST(Scene, TestShapeGetter) {
    Scene2f scene;
    scene.addShape(makeReferenceCounted<Disk2f>(identity<float>(), 10.0f));

    EXPECT_THAT(scene.getShapes().size(), 1u);
}

// todo: is this a camera test or scene test? should it be moved to camera test cpp file?
TEST(Scence, GivenAPointThatIsAlignedWithCameraSpace_WhenPointIsTransformedToCameraSpace_ThenExpectPointIsAtOrigin) {
    // Arrange
    auto scene = loadScene("scenes/rectangle.flatland.xml");
    Point2f worldSpacePoint(100.0f, 600.0f);

    // Act
    ReferenceCounted<Camera2f> camera = scene->getCamera();
    auto cameraSpacePoint = camera->getTransform() * worldSpacePoint;

    // Assert
    EXPECT_NEAR(cameraSpacePoint.x(), 0.0f, 0.0001f);
    EXPECT_NEAR(cameraSpacePoint.y(), 0.0f, 0.0001f);
    EXPECT_THAT(camera->getFilm().getWidth(), 600);
    EXPECT_THAT(camera->getFilm().getHeight(), 700);
}

TEST(Scence, TestMaterial) {
    // Arrange
    auto scene = loadScene("scenes/disk.flatland.xml");

    auto material = scene->getShapes()[0]->getMaterial();

    // Assert
    EXPECT_THAT(material->getStrokeWidth(), 3);
}

TEST(Scene, loadScene_WhenSceneFileDoesNotExist_ThenNullptr) {
    auto scene = loadScene("non_existing_file.xml");

    EXPECT_THAT(scene, nullptr);
}

TEST(Scene, loadScene_WhenSceneFileDoesExist_ThenExpectValidSceneObject) {
    auto scene = loadScene("scenes/disk.flatland.xml");

    EXPECT_NE(scene, nullptr);
}

TEST(Scene, GivenASceneFileWithCameraParameters_ThenExpectValidCameraParamters_WhenSceneIsLoaded) {
    auto scene = loadScene("scenes/disk.flatland.xml");

    EXPECT_THAT(scene->getCamera()->getFilm().getWidth(), 800);
    EXPECT_THAT(scene->getCamera()->getFilm().getHeight(), 600);
    EXPECT_THAT(scene->getCamera()->getFilm().getFilename(), "disk.svg");
}

TEST(Scene, loadScene_LoadLabels) {
    auto scene = loadScene("scenes/rectangle.flatland.xml");

    EXPECT_THAT(scene->getAnnotations().size(), 2u);
    EXPECT_THAT(scene->getAnnotations()[0].text, "Incident vector");
    EXPECT_THAT(scene->getAnnotations()[1].text, "Refracted vector");
}

TEST(Scene, loadScene_LoadPolygon2) {
    auto scene = loadScene("scenes/bunny.flatland.xml");
}

TEST(Scene, loadScene_MissingSceneTag) {
    EXPECT_THROW(loadScene("tests/scenes/scene_tag_missing.flatland.xml"), LoadSceneException);
}

TEST(Scene, loadScene_IntegratorMissing) {
    EXPECT_THROW(loadScene("tests/scenes/integrator_missing.flatland.xml"), IntegratorMissing);
}

TEST(Scene, ReadPolygonTransform2) {
    // Arrange
    auto scene = loadScene("scenes/bunny.flatland.xml");

    // Act
    auto transform = scene->getShapes()[0]->getTransform();

    // Assert
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
}