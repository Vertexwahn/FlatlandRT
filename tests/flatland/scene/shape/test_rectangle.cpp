/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/scene/shape/rectangle.h"

#include <gmock/gmock.h>

using namespace Flatland;

// Further ideas for tests:
// What happens if width= -5 -> Exception?
// What happens when infinite rectangle is defined?
// Does a infinite ray hit an infinite rectangle? yes at infinity...? or?
// NaN?

TEST(Rectangle2f, WhenInitializingRectangle_ThenExpectCorrectWidhtAndHeight) {
    constexpr float width = 2.0f;
    constexpr float height = 4.0f;

    PropertySet ps;
    ps.addProperty("width", width);
    ps.addProperty("height", height);
    ps.addProperty("transform", identity<float>());

    Rectangle2f rectangle{ps};
    EXPECT_THAT(rectangle.width(), width);
    EXPECT_THAT(rectangle.height(), height);
}

TEST(Rectangle2f, GivenAPreferedSizeForARectangle_WhenInitializing_ThenExpectedCorrectParameters) {
    // Arrange
    Point2f minimum{-25.0f, -50.0f};
    Point2f maximum{25.0f, 50.0f};
    auto width = maximum.x() - minimum.x();
    auto height = maximum.y() - minimum.y();

    PropertySet ps;
    ps.addProperty("width", width);
    ps.addProperty("height", height);
    ps.addProperty("transform", identity<float>());

    // Act
    Rectangle2f rect{ps};

    // Assert
    EXPECT_THAT(rect.width(), 50.0f);
    EXPECT_THAT(rect.height(), 100.0f);
    //EXPECT_THAT(rect.getCornerA(), minimum);
    //EXPECT_THAT(rect.getCornerC(), maximum);

    Matrix44f expectedMatrix;
    expectedMatrix << 1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f;

    EXPECT_THAT(rect.getTransform().getMatrix(), expectedMatrix);
}

TEST(Rectangle2f, GivenATranslatedRectangle_WhenInitializing_ThenExpectTransformedRectangle) {
    Point2f minimum{50.0f, 200.0f};
    Point2f maximum{200.0f, 300.0f};
    auto width = maximum.x() - minimum.x();
    auto height = maximum.y() - minimum.y();
    Point2f translation = minimum + Vector2f{width/2.0f, height/2.0f}; // todo: auto does not work here - why?

    PropertySet ps;
    ps.addProperty("width", width);
    ps.addProperty("height", height);
    ps.addProperty("transform", translate(translation.x(), translation.y()));

    Rectangle2f rect{ps};

    EXPECT_THAT(translation.x(), 125.0f);
    EXPECT_THAT(rect.width(), 150.0f);
    EXPECT_THAT(rect.height(), 100.0f);
    //EXPECT_THAT(rect.getCornerA(), minimum);
    //EXPECT_THAT(rect.getCornerC(), maximum);

    Matrix44f expectedMatrix;
    expectedMatrix << 1.0f, 0.0f, 0.0f, translation.x(),
            0.0f, 1.0f, 0.0f, translation.y(),
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f;

    EXPECT_THAT(rect.getTransform().getMatrix(), expectedMatrix);
}

TEST(Rectangle2f, WhenRayHitsRectangle_ExpectIntersection) {
    Vector2f position{250.0f, 250.0f};

    PropertySet ps;
    ps.addProperty("width", 100.0f);
    ps.addProperty("height", 100.0f);
    ps.addProperty("transform", translate(position));

    Rectangle2f rect{ps};

    Point2f rayOrigin{0.0f, 250.0f};
    Vector2f rayDirection{1.0f, 0.0f}; // shoot away - expected is no hit
    Ray2f ray{rayOrigin, rayDirection, 0.0f, 10.0f};

    MediumEvent2f its;
    bool result = rect.intersect(ray, its);

    EXPECT_TRUE(result);
    EXPECT_THAT(its.p.x(), 200.0f);
    EXPECT_THAT(its.p.y(), 250.0f);
    EXPECT_THAT(its.n.x(), -1.0f);
    EXPECT_THAT(its.n.y(), 0.0f);
}

TEST(Rectangl2f, GivenATranslatedRectangle_WhenConvertToSvg_ThenExpectSvgTags) {
    Vector2f position{250.0f, 250.0f};

    PropertySet ps;
    ps.addProperty("width", 100.0f);
    ps.addProperty("height", 100.0f);
    ps.addProperty("transform", translate(position));

    Rectangle2f rect{ps};

    EXPECT_THAT(rect.convertToSvg(500, 500), testing::HasSubstr("<path"));
}

TEST(Rectangle2f, GivenRectangleWithARotationTransform_WhenComputingACorner_ThenExpectRotatedCornerPoint) {
    constexpr float width = 400.0f;
    constexpr float height = 200.0f;
    auto transform = rotateZ(degreeToRadian(90.0f));

    PropertySet ps;
    ps.addProperty("width", width);
    ps.addProperty("height", height);
    ps.addProperty("transform", transform);

    Rectangle2f rectangle{ps};

    // Assert
    //auto px = -height/2.0f;
    //auto py = width/2.0f;

    std::string svgContent = rectangle.convertToSvg(800, 600);

    EXPECT_THAT(svgContent, testing::HasSubstr("100.0"));

    //Point2f a = rectangle.getCorner(3);
    //EXPECT_THAT(a.x(), testing::FloatEq(-height/2.0f));
    //EXPECT_THAT(a.y(), testing::FloatEq(width/2.0f));
}

/*
TEST(Rectangle2f, GivenRectangleWithARotationTransform_WhenComputingACorner_ThenExpectRotatedCornerPoint2) {
    constexpr float width = 400.0f;
    constexpr float height = 200.0f;
    auto transform = rotateZ(degreeToRadian(45.0f));

    Rectangle2f rectangle{transform, width, height};

    Point2f a = rectangle.getCornerC();

    EXPECT_THAT(a.x(), testing::FloatEq(70.7107f));
    EXPECT_THAT(a.y(), testing::FloatEq(212.132f));
}
*/
