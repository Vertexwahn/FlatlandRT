/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/scene/shape/rectangle.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(Rectangle2f, WhenInitializingRectangle_ThenExpectCorrectWidhtAndHeight) {
    constexpr  float width = 2.0f;
    constexpr float height = 4.0f;
    Rectangle2f rectangle{identity<float>(), width, height};
    EXPECT_THAT(rectangle.width(), width);
    EXPECT_THAT(rectangle.height(), height);
}

// todo what happens if width= -5 -> Exception
// what heppens when infinit rectangle is defined?
// does a infinit ray hit an inifite rectangle? yes at infinity...? or?
// NaN?
// shape

TEST(Rectangle2f, GivenAPreferedSizeForARectangle_WhenInitializing_ThenExpectedCorrectParameters) {
    // Arrange
    Point2f minimum{-25.0f, -50.0f};
    Point2f maximum{25.0f, 50.0f};
    auto width = maximum.x() - minimum.x();
    auto height = maximum.y() - minimum.y();

    // Act
    Rectangle2f rect{identity<float>(), width, height};

    // Assert
    EXPECT_THAT(rect.width(), 50.0f);
    EXPECT_THAT(rect.height(), 100.0f);
    EXPECT_THAT(rect.getMinimum(), minimum);
    EXPECT_THAT(rect.getMaximum(), maximum);

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
    Rectangle2f rect{translate(translation.x(), translation.y()), width, height};

    EXPECT_THAT(translation.x(), 125.0f);
    EXPECT_THAT(rect.width(), 150.0f);
    EXPECT_THAT(rect.height(), 100.0f);
    EXPECT_THAT(rect.getMinimum(), minimum);
    EXPECT_THAT(rect.getMaximum(), maximum);

    Matrix44f expectedMatrix;
    expectedMatrix << 1.0f, 0.0f, 0.0f, translation.x(),
            0.0f, 1.0f, 0.0f, translation.y(),
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f;

    EXPECT_THAT(rect.getTransform().getMatrix(), expectedMatrix);
}

TEST(Rectangle2f, WhenRayHitsRectangle_ExpectIntersection) {
    Vector2f position{250.0f, 250.0f};
    Rectangle2f rect{translate(position), 100.f, 100.0f};

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
    Rectangle2f rect{translate(position), 100.f, 100.0f};

    EXPECT_THAT(rect.convertToSvg(500, 500), ::testing::HasSubstr("<rect"));
    EXPECT_THAT(rect.convertToSvg(500, 500), ::testing::HasSubstr("</rect>"));
    // regex
    // http://www.regexe.de/hilfe.jsp
    // https://howtodoinjava.com/java/regex/start-end-of-string/
    EXPECT_THAT(rect.convertToSvg(500, 500), ::testing::MatchesRegex("^<rect.*>.*</rect>"));
}