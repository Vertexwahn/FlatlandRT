/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "math/util.h"
#include "math/transform.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(Transform44f, WhenDefaultInitalizingATransform_ExpectIdentityTransform) {
    // Act
    Transform44f transform;

    // Assert
    Matrix44f identity_matrix;
    identity_matrix << 1.f, 0.f, 0.f, 0.f,
                      0.f, 1.f, 0.f, 0.f,
                      0.f, 0.f, 1.f, 0.f,
                      0.f, 0.f, 0.f, 1.f;

    EXPECT_THAT(transform.matrix(), identity_matrix);
    EXPECT_THAT(transform.inverse(), identity_matrix);
}

TEST(Transform44f, GivenAMatrix_WhenInitalizingATransformWithThisMatrix_ExpectExactlyThisMatrix) {
    // Arrange
    Matrix44f matrix;
    matrix <<  1.f,  0.f, 0.f,  4.f,
               0.f,  1.f, 0.f,  8.f,
               0.f,  0.f, 1.f, 12.f,
               0.f,  0.f, 0.f,  1.f;

    // Act
    Transform44f transform(matrix);

    // Assert
    Matrix44f inverse;
    inverse << 1.f,  0.f, 0.f,  -4.f,
               0.f,  1.f, 0.f,  -8.f,
               0.f,  0.f, 1.f, -12.f,
               0.f,  0.f, 0.f,   1.f;

    EXPECT_THAT(transform.matrix(), matrix);
    EXPECT_THAT(transform.inverse().matrix(), inverse);
}

TEST(Transform44f, GivenMatrixAndInverse_WhenInitalizingTranform_ThenExpectCorrectValues) {
    // Arrange
    Matrix44f matrix;
    matrix <<  1.f,  0.f, 0.f,  4.f,
            0.f,  1.f, 0.f,  8.f,
            0.f,  0.f, 1.f, 12.f,
            0.f,  0.f, 0.f,  1.f;

    Matrix44f inverse;
    inverse << 1.f,  0.f, 0.f,  -4.f,
            0.f,  1.f, 0.f,  -8.f,
            0.f,  0.f, 1.f, -12.f,
            0.f,  0.f, 0.f,   1.f;

    // Act
    Transform44f transform(matrix, inverse);

    // Assert
    EXPECT_THAT(transform.matrix(), matrix);
    EXPECT_THAT(transform.inverse().matrix(), inverse);
}

TEST(Transform44f, GivenATranslateTransform_WhenConvertingToMatrix_ThenExpectTranslationMatrix) {
    auto transform = translate(1.f, 2.f);

    Matrix44f m = transform.matrix();

    Matrix44f expected_matrix;
    expected_matrix << 1.f, 0.f, 0.f, 1.f,
                      0.f, 1.f, 0.f, 2.f,
                      0.f, 0.f, 1.f, 0.f,
                      0.f, 0.f, 0.f, 1.f;

    EXPECT_THAT(m, expected_matrix);
}

TEST(Transform44f, GivenATranslateTransform_WhenConvertingToInverseMatrix_ThenExpectInverseTranslationMatrix) {
    auto transform = translate(1.f, 2.f);

    Matrix44f m = transform.inverse().matrix();

    Matrix44f expected_matrix;
    expected_matrix << 1.f, 0.f, 0.f, -1.f,
                      0.f, 1.f, 0.f, -2.f,
                      0.f, 0.f, 1.f, 0.f,
                      0.f, 0.f, 0.f, 1.f;

    EXPECT_THAT(m, expected_matrix);
}

TEST(Transform44f, Transform44f_multiplication) {
    Transform44f scaling(scale(3.f));
    Transform44f translation(translate(Vector2f(400.f, 300.f)));

    Transform44f result = translation * scaling;

    Matrix44f expected_result;
    expected_result << 3.f, 0.f, 0.f, 400.f,
                      0.f, 3.f, 0.f, 300.f,
                      0.f, 0.f, 3.f, 0.f,
                      0.f, 0.f, 0.f, 1.f;

    EXPECT_THAT(result.matrix(), expected_result);
}

TEST(Transform44f, Transform44f_assignment) {
    auto transform = identity<float>();
    transform = translate(2.f, 3.f) * transform;

    Matrix44f expected_result;
    expected_result << 1.f, 0.f, 0.f, 2.f,
                      0.f, 1.f, 0.f, 3.f,
                      0.f, 0.f, 1.f, 0.f,
                      0.f, 0.f, 0.f, 1.f;

    EXPECT_THAT(transform.matrix(), expected_result);
}

TEST(Transform44f, ComapreOperators) {
    auto transform1 = identity<float>();
    auto transform2 = translate(10.f, 10.f, 10.f);
    auto transform3 = translate(10.f, 10.f, 10.f);

    EXPECT_NE(transform1, transform2);
    EXPECT_EQ(transform2, transform3);
}

TEST(Transform44f, GivenRayInWorldSpace_ExpetRayInObjectSpace_WhenTransformedWithInverseOfWorldSpaceTransform) {
    auto translation = Vector2f{200.f, 100.f};
    auto to_world = translate(translation);
    Vector2f direction{1.f, 0.f};
    Point2f origin{0.f, 0.f};
    auto min_distance = 1.f;
    auto max_distance = 10.f;
    Ray2f r{origin, direction, min_distance, max_distance};
    
    auto toObjectSpace = to_world.inverse();
    Ray2f ray = toObjectSpace * r;

    EXPECT_THAT(ray.origin, (Point2f{-translation.x(), -translation.y()}));
    EXPECT_THAT(ray.direction, direction);
    EXPECT_THAT(ray.min_t, min_distance);
    EXPECT_THAT(ray.max_t, max_distance);
}

TEST(Transform44f, dir) {
    auto translation = Vector2f{400.f, 300.f};
    auto to_world = translate(translation) * scale(3.f, 3.f, 1.f);
    Vector2f direction{1.f, 0.f};
    Point2f origin{0.f, 0.f};
    auto min_distance = 1.f;
    auto max_distance = 10.f;
    Ray2f r{origin, direction, min_distance, max_distance};

    auto to_object_space = to_world.inverse();
    Ray2f ray = to_object_space * r;

    EXPECT_THAT(ray.origin, (Point2f{-translation.x()*1.f/3.f, -translation.y()*1.f/3.f}));
    //EXPECT_THAT(ray.direction, direction);
    EXPECT_THAT(ray.min_t, min_distance);
    EXPECT_THAT(ray.max_t, max_distance);
}

TEST(Transform44f, WhenTransformIsTranslation_ExpetedTranslatedPoint) {
    Point2f p(0.f, 0.f);

    auto transform = translate(1.f, 2.f);
    p = transform * p;

    EXPECT_THAT(p, (Point2f{1.f, 2.f}));
}

TEST(Transform44f, GivenAPoint_WhenApplyingAScaleTransform_ExpetedScaledPoint) {
    Point2f p{1.f, 1.f};
    auto transform = scale(10.f);

    p = transform * p;

    EXPECT_THAT(p, (Point2f{10.f, 10.f}));
}

TEST(Transform44f, GivenOriginAndTargetAlignedWithWorldSpace_ThenExpectIdentityMatrix) {
    // Arrange
    Point2f origin{0.f, 0.f};
    Point2f target{1.f, 0.f};

    // Act
    Transform44f t = look_at(origin, target);

    // Assert
    Matrix44f m = t.matrix();

    Matrix44f expected_matrix;
    expected_matrix << 1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f;

    EXPECT_THAT(m, expected_matrix);
}

TEST(Transform44f, GivenOriginAndTargetAlignedWithWorldSpace_ThenExpectIdentityMatrix_3D) {
    // Arrange
    Point3f origin{0.f, 0.f, 0.f};
    Point3f target{0.f, 0.f, 100.f};
    Vector3f up{0.f, 1.f, 0.f};

    // Act
    Transform44f t = look_at(origin, target, up);

    // Assert
    Matrix44f m = t.matrix();

    Matrix44f expected_matrix;
    expected_matrix << 1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f;

    EXPECT_THAT(m, expected_matrix);
}

TEST(Transform44f, look_at_invalid_up_vector) {
    // Arrange
    Point3f origin{0.f, 0.f, 0.f};
    Point3f target{0.f, 0.f, 100.f};
    Vector3f up{0.f, 0.f, 1.f};

    // Act & Assert
    EXPECT_THROW(look_at(origin, target, up), std::runtime_error);
}

TEST(Transform44f, GivenOriginAndTarget_WhenOriginTransformInCameraSpace_ThenOriginIsAtZero) {
    // Arrange
    Point2f origin{100.f, 100.f};
    Point2f target{200.f, 200.f};

    // Act
    Transform44f look_at_transform = look_at(origin, target);

    // Assert
    Vector2f forward = target - origin;
    Vector2f up{-forward.y(), forward.x()};
    forward.normalize();
    up.normalize();

    auto origin_in_camera_space = look_at_transform * origin;

    EXPECT_THAT(origin_in_camera_space.x(), testing::FloatNear(0.f, 0.00001f));
    EXPECT_THAT(origin_in_camera_space.y(), testing::FloatNear(0.f, 0.00001f));
}

TEST(Transform44f, lookat1) {
    Point2f eye{0.f, 0.f};
    Point2f target{1.f, 0.f};

    Transform44f transform = look_at(eye, target);

    Matrix44f expected_matrix;
    expected_matrix << 1.f,0.f,0.f,0.f,
                      0.f,1.f,0.f,0.f,
                      0.f,0.f,1.f,0.f,
                      0.f,0.f,0.f,1.f;

    EXPECT_TRUE(transform.matrix().isApprox(expected_matrix));
}

TEST(Transform44f, lookat2) {
    Point2f eye{3.f, 1.f};
    Point2f target{3.f, 2.f};

    Transform44f transform = look_at(eye, target);

    Matrix44f expected_matrix;
    expected_matrix << 0.f,1.f,0.f,-1.f,
                      -1.f,0.f,0.f, 3.f,
                       0.f,0.f,1.f, 0.f,
                       0.f,0.f,0.f, 1.f;

    EXPECT_TRUE(transform.matrix().isApprox(expected_matrix));
}

TEST(Transform44f, lookat3) {
    Point2f eye{3.f, 2.f};
    Point2f target{4.f, 3.f};

    Transform44f transform = look_at(eye, target);

    Matrix44f expected_matrix;
    expected_matrix << 0.707107f, 0.707107f, 0.f, -3.53553f,
                     -0.707107f, 0.707107f, 0.f, 0.707107f,
                           0.f,      0.f, 1.f, 0.f,
                           0.f,      0.f, 0.f, 1.f;

    EXPECT_TRUE(transform.matrix().isApprox(expected_matrix));
}

TEST(Transform44f, rotate_z) {
    auto rotation = rotate_z(degree_to_radian(90.f));

    Matrix44f expected_matrix;
    expected_matrix << 0.f, -1.f, 0.f, 0.f,
                      1.f,  0.f, 0.f, 0.f,
                      0.f,  0.f, 1.f, 0.f,
                      0.f,  0.f, 0.f, 1.f;

    EXPECT_TRUE(rotation.matrix().isApprox(expected_matrix));
}

TEST(Transform44f, rotate_x_zero_degrees) {
    auto rotation = rotate_x(degree_to_radian(0.f));

    Matrix44f expected_matrix;
    expected_matrix << 1.f,  0.f, 0.f, 0.f,
                       0.f,  1.f, 0.f, 0.f,
                       0.f,  0.f, 1.f, 0.f,
                       0.f,  0.f, 0.f, 1.f;

    EXPECT_TRUE(rotation.matrix().isApprox(expected_matrix));
}

TEST(Transform44f, rotate_x_90_degrees) {
    auto rotation = rotate_x(degree_to_radian(90.f));

    Matrix44f expected_matrix;
    expected_matrix << 1.f,  0.f, 0.f, 0.f,
            0.f,  0.f, -1.f, 0.f,
            0.f,  1.f, 0.f, 0.f,
            0.f,  0.f, 0.f, 1.f;

    EXPECT_TRUE(rotation.matrix().isApprox(expected_matrix));
}

TEST(Transform44f, WhenTransformIsScaling_ExpetedNotScaledVector) {
    Normal2f n{1.f, 0.f};
    auto transform = scale(10.f);

    n = transform * n;

    EXPECT_THAT(n.x(), testing::FloatEq(.1f));
    EXPECT_THAT(n.y(), testing::FloatEq(0.f));
}

TEST(Transform44f, WhenTransformIsRotatin_ExpetedRotatedNormalVector) {
    // Arrange
    auto transform = rotate_z(degree_to_radian(-45.f));
    Normal2f n{-1.f, 0.f};

    // Act
    n = transform * n;

    // Assert
    EXPECT_THAT(n.x(), testing::FloatEq(-sqrt(.5f)));
    EXPECT_THAT(n.y(), testing::FloatEq(sqrt(.5f)));
}

TEST(Transform44f, TestPoint2fMultiplication) {
    // Arrange
    Point2f p{1.f, 2.f};

    Matrix44f m;
    m << 1.f, 1.f, 1.f, 1.f,
         2.f, 2.f, 2.f, 2.f,
         3.f, 3.f, 3.f, 3.f,
         4.f, 4.f, 4.f, 4.f;

    Transform44f t{m};

    // Act
    auto pt = t * p;

    // Assert
    EXPECT_THAT(pt, (Point2f{4.f, 8.f}));
}

TEST(Transform44f, TestPoint3fMultiplication) {
    // Arrange
    Point3f p{1.f , 2.f, 3.f};

    Matrix44f m;
    m << 1.f, 1.f, 1.f, 1.f,
         2.f, 2.f, 2.f, 2.f,
         3.f, 3.f, 3.f, 3.f,
         4.f, 4.f, 4.f, 4.f;

    Transform44f t{m};

    // Act
    auto pt = t * p;

    // Assert
    EXPECT_THAT(pt, (Point3f{7.f/28.f, 14.f/28.f, 21.f/28.f}));
}

TEST(Transform44f, TestPoint4fMultiplication) {
    // Arrange
    Point4f p{1.f, 2.f, 3.f, 1.f};

    Matrix44f m;
    m << 1.f, 1.f, 1.f, 1.f,
         2.f, 2.f, 2.f, 2.f,
         3.f, 3.f, 3.f, 3.f,
         4.f, 4.f, 4.f, 4.f;

    Transform44f t{m};

    // Act
    auto pt = t * p;

    // Assert
    EXPECT_THAT(pt, (Point4f{7.f, 14.f, 21.f, 28.f}));
}

TEST(Transform44f, TestNormal3fMultiplication) {
    // Arrange
    Normal3f n{0.f , 1.f, 0.f};

    Transform44f t = translate(10.f, 20.f, 30.f) * scale(100.f);

    // Act
    auto nt = t * n;

    // Assert
    EXPECT_THAT(nt, (Normal3f{0.f, 1.f, 0.f}));
}

TEST(Transform44f, GivenFovNearAndFar_WhenCreatingAPespectiveTransformAndTranformAPoint_ThenExpectCorrectTransformedPoint) {
    auto transform = perspective(degree_to_radian(60.f), 10.f, 100.f);

    Point4f p1{0, 0, 10, 1};
    Point4f p2{0, 0, 100, 1};

    auto pt1 = transform * p1;
    auto pt2 = transform * p2;

    EXPECT_THAT(pt1.z() / pt1.w(), testing::FloatNear(0.f, 0.00001f));
    EXPECT_THAT(pt2.z() / pt2.w(), 1.f);
}

TEST(Transform44f, GivenFovNearAndFar_WhenCreatingAPespectiveTransform_ThenExpectCorrectMatrix) {
    auto transform = perspective(degree_to_radian(90.f), 10.f, 100.f);

    Matrix44f expected_matrix;
    expected_matrix << 1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 10.f/9.f, -100.0/9.0,
                      0, 0, 1, 0;

    EXPECT_TRUE(transform.matrix().isApprox(expected_matrix));
}

TEST(Transform44f, GivenFovNearAndFar_WhenCreatingAPespectiveTransform_ThenExpectCorrectMatrix2) {
    // Act
    auto n = 100.f;
    auto f = 500.f;
    auto transform = perspective(degree_to_radian(30.f), n, f);

    // Assert
    float s = 1 / (2 - sqrt(3));
    Matrix44f ms;
    ms   << s, 0, 0, 0,
            0, s, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;

    Matrix44f mp;
    mp   << 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 5.f/4.f, -500.f/4.f,
            0, 0, 1, 0;

    auto expected_matrix = ms * mp;

    EXPECT_TRUE(transform.matrix().isApprox(expected_matrix));
}

TEST(Transform44f, GivenAVector_WhenRotation0Degrees_ExpectSameVector) {
    // Arrange
    Vector4f v{0.f, 0.f, 1.f, 0.f};

    // Act
    auto transform = rotate_y(0.f);
    v = transform * v;

    // Assert
    EXPECT_THAT(v.x(), testing::FloatEq(0.f));
    EXPECT_THAT(v.y(), testing::FloatEq(0.f));
    EXPECT_THAT(v.z(), testing::FloatNear(1.f, 0.000001f));
}

TEST(Transform44f, GivenAVector_WhenRotation90Degrees_ExpectRotatedVector) {
    // Arrange
    Vector4f v{0.f, 0.f, 1.f, 0.f};

    // Act
    auto transform = rotate_y(degree_to_radian(90.f));
    v = transform * v;

    // Assert
    EXPECT_THAT(v.x(), testing::FloatEq(1.f));
    EXPECT_THAT(v.y(), testing::FloatEq(0.f));
    EXPECT_THAT(v.z(), testing::FloatNear(0.f, 0.000001f));
}

TEST(Transform44f, ostream) {
    Transform44f t = identity<float>();

    std::stringstream ss;
    ss << t;

    EXPECT_THAT(ss.str(), "Transform44f[\n  matrix = 1 0 0 0\n0 1 0 0\n0 0 1 0\n0 0 0 1]\n");
}
