/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/scene/shape/disk.h"
#include "flatland/math/util.h"
#include "flatland/math/refraction.h"
#include "flatland/canvas/svg_canvas.h"

#include <gmock/gmock.h>

#include <algorithm>
#include <cmath>

using namespace Flatland;

// For details see here:
// http://www.pbr-book.org/3ed-2018/Reflection_Models/Specular_Reflection_and_Transmission.html#Refract
TEST(RefractionVacuumToGlass, When_IncidentVectorIs45Degrees_Then_RefratedVectorIsAbout26Degrees) {
	// Arrange
	const Normal2f normal(0.0f, 1.0f);
    Vector2f incident(1.0f, -1.0f);
    Vector2f wi = -incident;
    wi.normalize();

	const auto refractionIndexVacuum = 1.0f;
	const auto refractionIndexGlass = 1.6f;

	// Act
	Vector2f refractedDirection;
	bool validRefraction = refract(wi, normal, refractionIndexVacuum / refractionIndexGlass, refractedDirection);

	// Assert
	EXPECT_TRUE(validRefraction);
	EXPECT_TRUE(refractedDirection.x() > 0.0f);
	EXPECT_TRUE(refractedDirection.y() < 0.0f);

	EXPECT_THAT(refractedDirection.norm(), testing::FloatEq(1.0f));

	float theta = degreeToRadian(26.23);
	Eigen::Rotation2Df rotationTransform(theta);
	Vector2f rotatedVector = rotationTransform.toRotationMatrix() * Vector2f(0.0, -1.0);

	EXPECT_THAT(refractedDirection.x(), testing::FloatNear(rotatedVector.x(), 0.0001f));
	EXPECT_THAT(refractedDirection.y(), testing::FloatNear(rotatedVector.y(), 0.0001f));
}

TEST(RefractionVacuumToGlass, When_IncidentVectorIs0Degrees_Then_RefratedVectorIs0Degrees) {
    // Arrange
    const Normal2f normal(0.0f, 1.0f);
    Vector2f incident(0.0f, -1.0f);
    Vector2f wi = -incident;
    wi.normalize();

    const auto refractionIndexVacuum = 1.0f;
    const auto refractionIndexGlass = 1.6f;

    // Act
    Vector2f refractedDirection;
    bool validRefraction = refract(wi, normal, refractionIndexVacuum / refractionIndexGlass, refractedDirection);

    // Assert
    EXPECT_TRUE(validRefraction);

    EXPECT_THAT(refractedDirection.norm(), testing::FloatEq(1.0f));

    float theta = degreeToRadian(0.0f);
    Eigen::Rotation2Df rotationTransform(theta);
    Vector2f rotatedVector = rotationTransform.toRotationMatrix() * Vector2f(0.0, -1.0);

    EXPECT_THAT(refractedDirection.x(), testing::FloatNear(rotatedVector.x(), 0.0001f));
    EXPECT_THAT(refractedDirection.y(), testing::FloatNear(rotatedVector.y(), 0.0001f));
}

TEST(RefractionGlassToVaccum, When_IncidentVectorIsCriticalAnglePlus5Degrees_Then_RefratedVectorIsInvalid) {
	// Arrange
	const Normal2f n(0.0f, 1.0f);
	Vector2f incident(-1.0f, 1.0f);
	incident.normalize();

	const auto refractionIndexVacuum = 1.0f;
	const auto refractionIndexGlass = 1.6f;

	const auto criticalAngle = std::asin(refractionIndexVacuum / refractionIndexGlass);

	const float theta = criticalAngle + degreeToRadian(5.0f);
	const Eigen::Rotation2Df t(theta);
	const Vector2f rotatedVector = t.toRotationMatrix() * Vector2f(0.0, 1.0);

	// Act
	Vector2f refractedDirection;
	bool validRefraction = refract(rotatedVector, n, refractionIndexGlass / refractionIndexVacuum, refractedDirection);

	// Assert
	EXPECT_FALSE(validRefraction);
}

TEST(RefractionRayHitsSphere, When_RayHitsSphereStraight_Then_ExpectStraightRefraction) {
	// Arrange
	const Ray2f r(Point2f(0.0f, 100.0f), Vector2f(1.0f, 0.0f), 0.0f, 100.0f);
	const Disk2f c(translate(Vector2f{100.0f, 100.0f}), 50.0f);

	MediumEvent2f its;
	c.intersect(r, its);

	Vector2f refractedDirection;

	Vector2f wo = -r.direction;

	// Act
	bool result = refract(wo, its.n, 1.0f / 1.60f, refractedDirection);

	// Assert
	ASSERT_TRUE(result);

	EXPECT_THAT(refractedDirection.dot(Vector2f(1.0f, 0.0f)), testing::FloatEq(1.0f));
}

TEST(RefractionRayHitsSphere, When_RayHitsSphereStraightFromInside_Then_ExpectStraightRefraction) {
	// Arrange
	Ray2f r(Point2f(100.0f, 100.0f), Vector2f(1.0f, 0.0f), 0.0f, 100.0f);
	Disk2f disk(translate(Vector2f{100.0f, 100.0f}), 50.0f);

	MediumEvent2f its;
	disk.intersect(r, its);

	Vector2f refractedDirection;
	Vector2f wo = -r.direction;

	// Act
	bool result = refract(wo, faceforward(its.n, wo), 1.0f / 1.6f, refractedDirection);

	// Assert
	ASSERT_TRUE(result);

	EXPECT_THAT(its.p.x(), testing::FloatEq(150.0f));
	EXPECT_THAT(its.p.y(), testing::FloatEq(100.0f));

	EXPECT_THAT(refractedDirection.dot(Vector2f(1.0f, 0.0f)), testing::FloatEq(1.0f));
}

TEST(RefractionRayHitsAndExitsSphere, When_RayFromTopAt45Degrees_HitsSphere) {
	SvgCanvas2f sc{800, 600};

	Ray2f r{Point2f{50.0f, 200.0f}, Vector2f{1.0f, -1.0f}.normalized(), 0.0f, 500.0f};
	Disk2f c{translate(Vector2f{100.0f, 100.0f}), 50.0f};

	sc.add(&c);

	MediumEvent2f its;
	c.intersect(r, its);

	// 100
	// 150
	EXPECT_THAT(its.p.x(), testing::FloatNear(100.0f, 0.001f));
	EXPECT_THAT(its.p.y(), testing::FloatNear(150.0f, 0.001f));
	std::cout << "pointA " << its.p << std::endl;

	r.max_t = its.t;

	sc.add(r);

	Vector2f refractedDirection;

	Vector2f wo1 = -r.direction;
	refract(wo1, its.n, 1.0f / 1.6f, refractedDirection);

	Ray2f refractedRay{its.p + refractedDirection * 0.01f, refractedDirection, 0.0f, 200};

	c.intersect(refractedRay, its);

	// 139.644
	// 69.5312
	EXPECT_THAT(its.p.x(), testing::FloatNear(139.644f, 0.001f));
	EXPECT_THAT(its.p.y(), testing::FloatNear(69.5312f, 0.001f));
	std::cout << "pointB " << its.p << std::endl;

	refractedRay.max_t = its.t;
	sc.add(refractedRay);

	Vector2f refractedDirection2;
	Vector2f wo = -refractedRay.direction;
	refract(wo, faceforward(its.n, wo), 1.6f / 1.0f, refractedDirection2);

	Ray2f refractedRay2{its.p, refractedDirection2, 0.0f, 50.0f};
	sc.add(refractedRay2);
}
