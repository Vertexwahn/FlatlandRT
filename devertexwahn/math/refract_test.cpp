/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "math/refract.h"
#include "math/util.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

// For details see here:
// http://www.pbr-book.org/3ed-2018/Reflection_Models/Specular_Reflection_and_Transmission.html#Refract
TEST(RefractionVacuumToGlass,
     Given_IncidentVectorIs45Degrees_When_ComputingRefraction_Then_RefratedVectorIsAbout26Degrees) {
    // Arrange
    const Normal2f normal{0.f, 1.f};
    Vector2f incident{1.f, -1.f};
    Vector2f wi = -incident;
    wi.normalize();

    const auto refraction_index_vacuum = 1.f;
    const auto refraction_index_glass = 1.6f;

    // Act
    Vector2f refracted_direction;
    bool valid_refraction = refract(wi, normal, refraction_index_vacuum / refraction_index_glass, refracted_direction);

    // Assert
    EXPECT_TRUE(valid_refraction);
    EXPECT_TRUE(refracted_direction.x() > 0.f);
    EXPECT_TRUE(refracted_direction.y() < 0.f);

    EXPECT_THAT(refracted_direction.norm(), testing::FloatEq(1.f));

    float theta = degree_to_radian(26.23f);
    Eigen::Rotation2Df rotation_transform(theta);
    Vector2f rotatedVector = rotation_transform.toRotationMatrix() * Vector2f{0.f, -1.f};

    EXPECT_THAT(refracted_direction.x(), testing::FloatNear(rotatedVector.x(), .0001f));
    EXPECT_THAT(refracted_direction.y(), testing::FloatNear(rotatedVector.y(), .0001f));
}

TEST(RefractionVacuumToGlass, When_IncidentVectorIs0Degrees_Then_RefratedVectorIs0Degrees) {
    // Arrange
    const Normal2f normal{0.f, 1.f};
    Vector2f incident{0.f, -1.f};
    Vector2f wi = -incident;
    wi.normalize();

    const auto refraction_index_vacuum = 1.f;
    const auto refraction_index_glass = 1.6f;

    // Act
    Vector2f refracted_direction;
    bool valid_refraction = refract(wi, normal, refraction_index_vacuum / refraction_index_glass, refracted_direction);

    // Assert
    EXPECT_TRUE(valid_refraction);

    EXPECT_THAT(refracted_direction.norm(), testing::FloatEq(1.f));

    float theta = degree_to_radian(0.f);
    Eigen::Rotation2Df rotation_transform(theta);
    Vector2f rotatedVector = rotation_transform.toRotationMatrix() * Vector2f{.0, -1.0};

    EXPECT_THAT(refracted_direction.x(), testing::FloatNear(rotatedVector.x(), 0.0001f));
    EXPECT_THAT(refracted_direction.y(), testing::FloatNear(rotatedVector.y(), 0.0001f));
}

TEST(RefractionGlassToVaccum, When_IncidentVectorIsCriticalAnglePlus5Degrees_Then_RefratedVectorIsInvalid) {
    // Arrange
    const Normal2f n{0.f, 1.f};
    Vector2f incident{-1.f, 1.f};
    incident.normalize();

    const auto refraction_index_vacuum = 1.f;
    const auto refraction_index_glass = 1.6f;

    const auto criticalAngle = std::asin(refraction_index_vacuum / refraction_index_glass);

    const float theta = criticalAngle + degree_to_radian(5.f);
    const Eigen::Rotation2Df t(theta);
    const Vector2f rotated_vector = t.toRotationMatrix() * Vector2f(.0, 1.0);

    // Act
    Vector2f refracted_direction;
    bool valid_refraction = refract(rotated_vector, n, refraction_index_glass / refraction_index_vacuum,
                                    refracted_direction);

    // Assert
    EXPECT_FALSE(valid_refraction);
}
