/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/shape/rectangle.hpp"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(Rectangle2f, GivenApreferredSizeForARectangle_WhenInitializing_ThenExpectedCorrectParameters) {
    // Arrange
    Point2f minimum{-25.f, -50.f};
    Point2f maximum{25.f, 50.f};
    auto width = maximum.x() - minimum.x();
    auto height = maximum.y() - minimum.y();

    PropertySet ps;
    ps.add_property("width", width);
    ps.add_property("height", height);
    ps.add_property("transform", identity<float>());

    // Act
    Rectangle2f rect{ps};

    // Assert
    Matrix44f expected_matrix;
    expected_matrix << 1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f;

    EXPECT_THAT(rect.transform().matrix(), expected_matrix);
}

TEST(Rectangle2f, GivenATranslatedRectangle_WhenInitializing_ThenExpectTransformedRectangle) {
    Point2f minimum{50.f, 200.f};
    Point2f maximum{200.f, 300.f};
    auto width = maximum.x() - minimum.x();
    auto height = maximum.y() - minimum.y();
    Point2f translation = minimum + Vector2f{width / 2.f, height / 2.f}; // todo: auto does not work here - why?

    PropertySet ps;
    ps.add_property("width", width);
    ps.add_property("height", height);
    ps.add_property("transform", translate(translation.x(), translation.y()));

    Rectangle2f rect{ps};

    EXPECT_THAT(translation.x(), 125.f);

    Matrix44f expected_matrix;
    expected_matrix << 1.f, 0.f, 0.f, translation.x(),
            0.f, 1.f, 0.f, translation.y(),
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f;

    EXPECT_THAT(rect.transform().matrix(), expected_matrix);
}

TEST(Rectangle2f, Given_RayHitsRectangle_When_ComputingIntersection_ExpectIntersection) {
    Vector2f position{250.f, 250.f};

    PropertySet ps;
    ps.add_property("width", 100.f);
    ps.add_property("height", 100.f);
    ps.add_property("transform", translate(position));

    Rectangle2f rect{ps};

    Point2f ray_origin{0.f, 250.f};
    Vector2f ray_direction{1.f, 0.f}; // shoot away - expected is no hit
    Ray2f ray{ray_origin, ray_direction, 0.f, 10.f};

    MediumEvent2f its;
    bool result = rect.intersect(ray, its);

    EXPECT_TRUE(result);
    EXPECT_THAT(its.p.x(), 200.f);
    EXPECT_THAT(its.p.y(), 250.f);
    EXPECT_THAT(its.geo_frame.n.x(), -1.f);
    EXPECT_THAT(its.geo_frame.n.y(), 0.f);
}

TEST(Rectangl2f, GivenATranslatedRectangle_WhenConvertToSvg_ThenExpectSvgTags) {
    Vector2f position{250.f, 250.f};

    PropertySet ps;
    ps.add_property("width", 100.f);
    ps.add_property("height", 100.f);
    ps.add_property("transform", translate(position));

    Rectangle2f rect{ps};

    EXPECT_THAT(rect.convert_to_svg(500, 500), testing::HasSubstr("<path"));
}

TEST(Rectangle2f, GivenRectangleWithArotation_transform_WhenComputingACorner_ThenExpectRotatedCornerPoint) {
    constexpr float width = 400.f;
    constexpr float height = 200.f;
    auto transform = rotate_z(degree_to_radian(90.f));

    PropertySet ps;
    ps.add_property("width", width);
    ps.add_property("height", height);
    ps.add_property("transform", transform);

    Rectangle2f rectangle{ps};

    // Assert
    //auto px = -height/2.f;
    //auto py = width/2.f;

    std::string svgContent = rectangle.convert_to_svg(800, 600);

    EXPECT_THAT(svgContent, testing::HasSubstr("100.0"));

    //Point2f a = rectangle.getCorner(3);
    //EXPECT_THAT(a.x(), testing::FloatEq(-height/2.f));
    //EXPECT_THAT(a.y(), testing::FloatEq(width/2.f));
}

/*
TEST(Rectangle2f, GivenRectangleWithArotation_transform_WhenComputingACorner_ThenExpectRotatedCornerPoint2) {
    constexpr float width = 400.f;
    constexpr float height = 200.f;
    auto transform = rotateZ(degreeToRadian(45.f));

    Rectangle2f rectangle{transform, width, height};

    Point2f a = rectangle.getCornerC();

    EXPECT_THAT(a.x(), testing::FloatEq(70.7107f));
    EXPECT_THAT(a.y(), testing::FloatEq(212.132f));
}
*/
