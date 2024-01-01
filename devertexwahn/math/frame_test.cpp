/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "math/frame.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(Frame, compute_orthonormal_basis) {
    // Arrange
    Normal3f n{0.f, 0.f, 1.f};
    Vector3f b1, b2;

    // Act
    revised_onb(n, b1, b2);

    // Assert
    auto computed_n = b1.cross(b2);
    Normal3f expected_n{0.f, 0.f, 1.f};
    EXPECT_THAT(computed_n.x(), testing::FloatNear(expected_n.x(), .00001f));
    EXPECT_THAT(computed_n.y(), testing::FloatNear(expected_n.y(), .00001f));
    EXPECT_THAT(computed_n.z(), testing::FloatNear(expected_n.z(), .00001f));
}

TEST(Frame, GivenFrameIdenticalWithWorld_WhenTransformToWorld_ExpectSameCoordinates) {
    // Arrange
    Frame3f frame{{1.f, 0.f, 0.f},
                  {0.f, 1.f, 0.f},
                  {0.f, 0.f, 1.f}};
    Vector3f v{1.f, 2.f, 3.f};

    // Act
    auto result = frame.to_world(v);

    // Assert
    EXPECT_THAT(result, v);
}

TEST(Frame, abs_cos_theta) {
    EXPECT_THAT(abs_cos_theta(Vector3f{0.f, 1.f, 0.f}), 0.f);
    EXPECT_THAT(abs_cos_theta(Vector3f{0.f, 0.f, 1.f}), 1.f);
}
