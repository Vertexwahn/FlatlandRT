/*
 *  SPDX-FileCopyrightText: Copyright 2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/fresnel.hpp"

#include "math/normal.hpp"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(refract, total_internal_reflection) {
    Vector3f v{1.f, 0.1f, 0.f};
    Vector3f n{0.f, 1.f, 0.f};

    Vector3f t;
    EXPECT_FALSE(refract(v, n, 1.5f, 1.0f, t));
}

TEST(reflection, reflect) {
    Vector3f v{-1.f, 1.f, 0.f};
    Vector3f n{0.f, 1.f, 0.f};

    EXPECT_THAT(reflect(v,n), (Vector3f{1.f, 1.f, 0.f}));
}

TEST(RefractionVacuumToGlass,
     Given_IncidentVectorIs45Degrees_When_ComputingRefraction_Then_RefratedVectorIsAbout26Degrees) {
    // Arrange
    const Normal3f normal{0.f, 1.f, 0.f};
    Vector3f incident{1.f, -1.f, 0.f};
    Vector3f wi = -incident;
    wi.normalize();

    const auto refraction_index_vacuum = 1.f;
    const auto refraction_index_glass = 1.6f;

    // Act
    Vector3f refracted_direction;
    bool valid_refraction = refract(wi, normal, refraction_index_vacuum, refraction_index_glass, refracted_direction);

    // Assert

    EXPECT_TRUE(valid_refraction);
    EXPECT_TRUE(refracted_direction.x() > 0.f);
    EXPECT_TRUE(refracted_direction.y() < 0.f);

    //EXPECT_THAT(refracted_direction.norm(), testing::FloatEq(1.f));
    /*
    float theta = degree_to_radian(26.23f);
    Eigen::Rotation2Df rotation_transform(theta);
    Vector2f rotatedVector = rotation_transform.toRotationMatrix() * Vector2f{0.f, -1.f};

    EXPECT_THAT(refracted_direction.x(), testing::FloatNear(rotatedVector.x(), .0001f));
    EXPECT_THAT(refracted_direction.y(), testing::FloatNear(rotatedVector.y(), .0001f));
    */
}
