/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/math/util.h"
#include "flatland/math/transform.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(Transform44f, WhenDefaultInitalizingATransform_ExpectIdentityTransform) {
    // Act
    Transform44f transform;

    // Assert
    Matrix44f identityMatrix;
    identityMatrix << 1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f;

    EXPECT_THAT(transform.getMatrix(), identityMatrix);
    EXPECT_THAT(transform.inverse(), identityMatrix);
}

TEST(Transform44f, GivenAMatrix_WhenInitalizingATransformWithThisMatrix_ExpectExactlyThisMatrix) {
    // Arrange
    Matrix44f matrix;
    matrix <<  1.0f,  0.0f, 0.0f,  4.0f,
               0.0f,  1.0f, 0.0f,  8.0f,
               0.0f,  0.0f, 1.0f, 12.0f,
               0.0f,  0.0f, 0.0f,  1.0f;

    // Act
    Transform44f transform(matrix);

    // Assert
    Matrix44f inverse;
    inverse << 1.0f,  0.0f, 0.0f,  -4.0f,
               0.0f,  1.0f, 0.0f,  -8.0f,
               0.0f,  0.0f, 1.0f, -12.0f,
               0.0f,  0.0f, 0.0f,   1.0f;

    EXPECT_THAT(transform.getMatrix(), matrix);
    EXPECT_THAT(transform.inverse().getMatrix(), inverse);
}

TEST(Transform44f, a) {
    // Arrange
    Matrix44f matrix;
    matrix <<  1.0f,  0.0f, 0.0f,  4.0f,
            0.0f,  1.0f, 0.0f,  8.0f,
            0.0f,  0.0f, 1.0f, 12.0f,
            0.0f,  0.0f, 0.0f,  1.0f;

    Matrix44f inverse;
    inverse << 1.0f,  0.0f, 0.0f,  -4.0f,
            0.0f,  1.0f, 0.0f,  -8.0f,
            0.0f,  0.0f, 1.0f, -12.0f,
            0.0f,  0.0f, 0.0f,   1.0f;

    // Act
    Transform44f transform(matrix, inverse);
    //ASSERT_DEATH(
    // Assert
    EXPECT_THAT(transform.getMatrix(), matrix);
    EXPECT_THAT(transform.inverse().getMatrix(), inverse);
}

//------------------------------------------------------------------------

TEST(Transform44f, GivenATranslateTransform_WhenConvertingToMatrix_ThenExpectTranslationMatrix) {
    auto transform = translate(1.0f, 2.0f);

    Matrix44f m = transform.getMatrix();

    Matrix44f expectedMatrix;
    expectedMatrix << 1.0f, 0.0f, 0.0f, 1.0f,
                      0.0f, 1.0f, 0.0f, 2.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f;

    EXPECT_THAT(m, expectedMatrix);
}

TEST(Transform44f, GivenATranslateTransform_WhenConvertingToInverseMatrix_ThenExpectInverseTranslationMatrix) {
    auto transform = translate(1.0f, 2.0f);

    Matrix44f m = transform.inverse().getMatrix();

    Matrix44f expectedMatrix;
    expectedMatrix << 1.0f, 0.0f, 0.0f, -1.0f,
                      0.0f, 1.0f, 0.0f, -2.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f;

    EXPECT_THAT(m, expectedMatrix);
}

TEST(Transform44f, Transform44f_multiplication) {
    Transform44f scaling(scale(3.0f));
    Transform44f translation(translate(Vector2f(400.0f, 300.0f)));

    Transform44f result = translation * scaling;

    Matrix44f expectedResult;
    expectedResult << 3.0f, 0.0f, 0.0f, 400.0f,
                      0.0f, 3.0f, 0.0f, 300.0f,
                      0.0f, 0.0f, 3.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f;

    EXPECT_THAT(result.getMatrix(), expectedResult);
}

TEST(Transform44f, Transform44f_assignment) {
    auto transform = identity<float>();
    transform = translate(2.0f, 3.0f) * transform;

    Matrix44f expectedResult;
    expectedResult << 1.0f, 0.0f, 0.0f, 2.0f,
                      0.0f, 1.0f, 0.0f, 3.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f;

    EXPECT_THAT(transform.getMatrix(), expectedResult);
}

TEST(Transform44f, GivenRayInWorldSpace_ExpetRayInObjectSpace_WhenTransformedWithInverseOfWorldSpaceTransform) {
    auto translation = Vector2f{200.0f, 100.0f};
    auto toWorld = translate(translation);
    Vector2f direction{1.0f, 0.0f};
    Point2f origin{0.0f, 0.0f};
    auto minDistance = 1.0f;
    auto maxDistance = 10.0f;
    Ray2f r{origin, direction, minDistance, maxDistance};


    auto toObjectSpace = toWorld.inverse();
    Ray2f ray = toObjectSpace * r;

    EXPECT_THAT(ray.origin, (Point2f{-translation.x(), -translation.y()}));
    EXPECT_THAT(ray.direction, direction);
    EXPECT_THAT(ray.min_t, minDistance);
    EXPECT_THAT(ray.max_t, maxDistance);
}

TEST(Transform44f, dir) {
    auto translation = Vector2f{400.0f, 300.0f};
    auto toWorld = translate(translation) * scale(3.0f, 3.0f, 1.0f);
    Vector2f direction{1.0f, 0.0f};
    Point2f origin{0.0f, 0.0f};
    auto minDistance = 1.0f;
    auto maxDistance = 10.0f;
    Ray2f r{origin, direction, minDistance, maxDistance};

    auto toObjectSpace = toWorld.inverse();
    Ray2f ray = toObjectSpace * r;

    EXPECT_THAT(ray.origin, (Point2f{-translation.x()*1.0f/3.0f, -translation.y()*1.0f/3.0f}));
    //EXPECT_THAT(ray.direction, direction);
    EXPECT_THAT(ray.min_t, minDistance);
    EXPECT_THAT(ray.max_t, maxDistance);
}

TEST(Transform44f, WhenTransformIsTranslation_ExpetedTranslatedPoint) {
    Point2f p(0.0f, 0.0f);

    auto transform = translate(1.0f, 2.0f);
    p = transform * p;

    EXPECT_THAT(p, (Point2f{1.0f, 2.0f}));
}

TEST(Transform44f, GivenAPoint_WhenApplyingAScaleTransform_ExpetedScaledPoint) {
    Point2f p{1.0f, 1.0f};
    auto transform = scale(10.0f);

    p = transform * p;

    EXPECT_THAT(p, (Point2f{10.0f, 10.0f}));
}

TEST(Transform44f, GivenOriginAndTargetAlignedWithWorldSpace_ThenExpectIdentityMatrix) {
    // Arrange
    Point2f origin{0.0f, 0.0f};
    Point2f target{1.0f, 0.0f};

    // Act
    Transform44f t = look_at(origin, target);

    // Assert
    Matrix44f m = t.getMatrix();

    Matrix44f expectedMatrix;
    expectedMatrix << 1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f;

    EXPECT_THAT(m, expectedMatrix);
}

TEST(Transform44f, GivenOriginAndTarget_WhenOriginTransformInCameraSpace_ThenOriginIsAtZero) {
    // Arrange
    Point2f origin{100.0f, 100.0f};
    Point2f target{200.0f, 200.0f};

    // Act
    Transform44f t = look_at(origin, target);

    // Assert
    Vector2f forward = target - origin;
    Vector2f up{-forward.y(), forward.x()};
    forward.normalize();
    up.normalize();

    //std::cout << "look_at: " << look_at(origin, target) << std::endl;

    auto originInCameraSpace = t * origin;

    EXPECT_THAT(originInCameraSpace, (Point2f{0.0f, 0.0f}));
}

TEST(Transform44f, lookat1) {
    Point2f eye{0.0f, 0.0f};
    Point2f target{1.0f, 0.0f};

    Transform44f transform = look_at(eye, target);

    Matrix44f expectedMatrix;
    expectedMatrix << 1.0f,0.0f,0.0f,0.0f,
                      0.0f,1.0f,0.0f,0.0f,
                      0.0f,0.0f,1.0f,0.0f,
                      0.0f,0.0f,0.0f,1.0f;

    ASSERT_TRUE(transform.getMatrix().isApprox(expectedMatrix));
}

TEST(Transform44f, lookat2) {
    Point2f eye{3.0f, 1.0f};
    Point2f target{3.0f, 2.0f};

    Transform44f transform = look_at(eye, target);

    Matrix44f expectedMatrix;
    expectedMatrix <<  0.0f,1.0f,0.0f,-1.0f,
                      -1.0f,0.0f,0.0f, 3.0f,
                       0.0f,0.0f,1.0f, 0.0f,
                       0.0f,0.0f,0.0f, 1.0f;

    ASSERT_TRUE(transform.getMatrix().isApprox(expectedMatrix));
}

TEST(Transform44f, lookat3) {
    Point2f eye{3.0f, 2.0f};
    Point2f target{4.0f, 3.0f};

    Transform44f transform = look_at(eye, target);

    Matrix44f expectedMatrix;
    expectedMatrix << 0.707107f, 0.707107f, 0.0f, -3.53553f,
                     -0.707107f, 0.707107f, 0.0f, 0.707107f,
                           0.0f,      0.0f, 1.0f, 0.0f,
                           0.0f,      0.0f, 0.0f, 1.0f;

    ASSERT_TRUE(transform.getMatrix().isApprox(expectedMatrix));
}

TEST(Transform44f, rotateZ) {
    auto rotation = rotateZ(degreeToRadian(90.0f));

    Matrix44f expectedMatrix;
    expectedMatrix << 0.0f, -1.0f, 0.0f, 0.0f,
                      1.0f,  0.0f, 0.0f, 0.0f,
                      0.0f,  0.0f, 1.0f, 0.0f,
                      0.0f,  0.0f, 0.0f, 1.0f;

    ASSERT_TRUE(rotation.getMatrix().isApprox(expectedMatrix));
}

TEST(Transform44f, WhenTransformIsScaling_ExpetedNotScaledVector) {
    Normal2f n{1.0f, 0.0f};
    auto transform = scale(10.0f);

    n = transform * n;

    EXPECT_THAT(n.x(), testing::FloatEq(0.1f));
    EXPECT_THAT(n.y(), testing::FloatEq(0.0f));
}

TEST(Transform44f, WhenTransformIsRotatin_ExpetedRotatedNormalVector) {
    // Arrange
    auto transform = rotateZ(degreeToRadian(-45.0f));
    Normal2f n{-1.0f, 0.0f};

    // Act
    n = transform * n;

    // Assert
    EXPECT_THAT(n.x(), testing::FloatEq(-sqrt(0.5f)));
    EXPECT_THAT(n.y(), testing::FloatEq(sqrt(0.5f)));
}