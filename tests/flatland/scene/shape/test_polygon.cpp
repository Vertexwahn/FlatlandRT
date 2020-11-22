/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/math/geometry.h"
#include "flatland/math/util.h"
#include "flatland/scene/shape/polygon.h"

#include <gmock/gmock.h>

using namespace Flatland;

class Polygon2fTest : public testing::Test {
protected:
    virtual void SetUp()
    {
        std::vector<Point2f> points = createSquare(100.0f);

        auto transform = translate(Vector2f{100.0f, 100.0f});

        auto pointCount = points.size();
        polygon = makeReferenceCounted<Polygon2f>(transform, &points[0], pointCount);
    }

    ReferenceCounted<Polygon2f> polygon = nullptr;
};

TEST_F(Polygon2fTest, GivenAPolygonAndRay_WhenRayIntesectsPolygon_ThenIntersectionOnPolygon) {
    // Arrange
    Point2f rayOrigin{0.0f, 100.0f};
    Vector2f rayDirection{1.0f, 0.0f};
    Ray2f ray{rayOrigin, rayDirection, 0.0f, 10000.0f};

    // Act
    MediumEvent2f me;
    auto hit = polygon->intersect(ray, me);

    // Assert
    EXPECT_TRUE(hit);

    EXPECT_THAT(me.frame.normal.x(), testing::FloatEq(-1.0f));
    EXPECT_THAT(me.frame.normal.y(), testing::FloatEq(0.0f));
    EXPECT_THAT(me.frame.tangent.x(), testing::FloatEq(0.0f));
    EXPECT_THAT(me.frame.tangent.y(), testing::FloatEq(1.0f));
}

TEST_F(Polygon2fTest, GivenAPolygonAndRay_WhenRayMissesPolygon_ThenNoIntersectionOnPolygon) {
    Point2f rayOrigin{0.0f, 0.0f};
    Vector2f rayDirection{1.0f, 0.0f};
    Ray2f ray{rayOrigin, rayDirection, 0.0f, 10000.0f};

    // Act
    MediumEvent2f me;
    auto hit = polygon->intersect(ray, me);

    // Assert
    EXPECT_FALSE(hit);
}

TEST(Polygon2f, GivenTranslatedPolygon_WhenConvertedToSvg_ThenExpectTranslationInSvgString) {
    std::vector<Point2f> points = {
            {0.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 15.0f}
    };

    auto transform = translate(Vector2f(100.0f, 100.0f));

    size_t pointCount = points.size();
    Polygon2f p(transform, &points[0], pointCount);

    std::string svgString = p.convertToSvg(800, 600);

    EXPECT_THAT(svgString, testing::HasSubstr("100"));
}

TEST(Polygon2f, WhenConvertingToSvg_ThenExpectSvgPathTag) {
    auto points = createStanfordBunnyPointList<float>();

    Transform44f transform = translate(Vector2f(100.0f, 100.0f));

    size_t pointCount = points.size();
    Polygon2f p(transform, &points[0], pointCount);

    std::string svgString = p.convertToSvg(800, 600);

    EXPECT_THAT(svgString, testing::HasSubstr("<path"));
    EXPECT_THAT(svgString, testing::HasSubstr(" Z\""));
    EXPECT_THAT(svgString, testing::HasSubstr(">"));
    EXPECT_THAT(svgString, testing::HasSubstr("115.260"));
}

TEST(Polygon, AssumePlausibleNormal) {
    std::vector<Point2f> points = createStanfordBunnyPointList<float>();

    auto transform = translate(Vector2f{400.0f-50.0f, 300.0f-50.0f});
    auto pointCount = points.size();
    auto polygon = makeReferenceCounted<Polygon2f>(transform, &points[0], pointCount);

    Point2f rayOrigin{200.0f, 500.0f};
    Vector2f rayDirection{1.0f, -1.0f};
    rayDirection.normalize();

    Ray2f ray{rayOrigin, rayDirection, 0.0f, 10000.0f};

    // Act
    MediumEvent2f its;
    auto hit = polygon->intersect(ray, its);

    // Assert
    EXPECT_TRUE(hit);
    EXPECT_GE(its.p.x(), 100.0f);
    EXPECT_LE(its.n.x(), 0.0f);
    EXPECT_GE(its.n.y(), 0.0f);
}

TEST(Polygon2f, GivenARotateCube_WhenComputingIntersection_ThenAssumePlausibeNormals) {
    // Arrange
    std::vector<Point2f> points = createSquare(100.0f);

    auto transform = translate(Vector2f{400.0f, 400.0f}) * rotateZ(degreeToRadian(-45.0f));
    auto polygon = makeReferenceCounted<Polygon2f>(transform, &points[0], points.size());

    Point2f rayOrigin{200.0f, 600.0f};
    Vector2f rayDirection{1.0f, -1.0f};
    rayDirection.normalize();

    Ray2f ray{rayOrigin, rayDirection, 0.0f, 10000.0f};

    // Act
    MediumEvent2f its;
    auto hit = polygon->intersect(ray, its);

    // Assert
    EXPECT_TRUE(hit);
    EXPECT_THAT(its.n.x(), testing::FloatEq(-sqrt(0.5f)));
    EXPECT_THAT(its.n.y(), testing::FloatEq(sqrt(0.5f)));
}

TEST(Polygon2f, GivenScaledPolygonCube_WhenRayIntersectsPolygon_AssumeProperDistance) {
    // Arrange
    std::vector<Point2f> points = createSquare(10.0f);

    auto transform = translate<float>(Vector2f{400.0f, 400.0f}) * scale<float>(20);
    auto polygon = makeReferenceCounted<Polygon2f>(transform, &points[0], points.size());

    Point2f rayOrigin{0.0f, 400.0f};
    Vector2f rayDirection{1.0f, 0.0f};
    rayDirection.normalize();

    Ray2f ray{rayOrigin, rayDirection, 0.0f, 10000.0f};

    // Act
    MediumEvent2f its;
    auto hit = polygon->intersect(ray, its);

    // Assert
    EXPECT_TRUE(hit);
    EXPECT_THAT(its.t, testing::FloatEq(300.0f));
}
