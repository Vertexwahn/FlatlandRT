/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "math/geometry.h"
#include "math/util.h"
#include "flatland/rendering/scene/shape/polygon.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

class Polygon2fTest : public testing::Test {
protected:
    virtual void SetUp() {
        std::vector<Point2f> points = create_square(100.f);

        auto transform = translate(Vector2f{100.f, 100.f});

        auto point_count = points.size();
        polygon = make_reference_counted<Polygon2f>(transform, &points[0], point_count);
    }

    ReferenceCounted<Polygon2f> polygon = nullptr;
};

TEST_F(Polygon2fTest, GivenAPolygonAndRay_WhenRayIntesectsPolygon_ThenIntersectionOnPolygon) {
    // Arrange
    Point2f ray_origin{0.f, 100.f};
    Vector2f ray_direction{1.f, 0.f};
    Ray2f ray{ray_origin, ray_direction, 0.f, 10000.f};

    // Act
    MediumEvent2f me;
    auto hit = polygon->intersect(ray, me);

    // Assert
    EXPECT_TRUE(hit);

    EXPECT_THAT(me.geo_frame.normal.x(), testing::FloatEq(-1.f));
    EXPECT_THAT(me.geo_frame.normal.y(), testing::FloatEq(0.f));
    EXPECT_THAT(me.geo_frame.tangent.x(), testing::FloatEq(0.f));
    EXPECT_THAT(me.geo_frame.tangent.y(), testing::FloatEq(1.f));
}

TEST_F(Polygon2fTest, GivenAPolygonAndRay_WhenRayMissesPolygon_ThenNoIntersectionOnPolygon) {
    Point2f ray_origin{0.f, 0.f};
    Vector2f ray_direction{1.f, 0.f};
    Ray2f ray{ray_origin, ray_direction, 0.f, 10000.f};

    // Act
    MediumEvent2f me;
    auto hit = polygon->intersect(ray, me);

    // Assert
    EXPECT_FALSE(hit);
}

TEST(Polygon2f, GivenTranslatedPolygon_WhenConvertedToSvg_ThenExpectTranslationInsvg_string) {
    std::vector<Point2f> points = {
            {0.f, 0.f},
            {1.f, 1.f},
            {0.f, 15.f}
    };

    auto transform = translate(Vector2f(100.f, 100.f));

    size_t point_count = points.size();
    Polygon2f p(transform, &points[0], point_count);

    std::string svg_string = p.convert_to_svg(800, 600);

    EXPECT_THAT(svg_string, testing::HasSubstr("100"));
}

TEST(Polygon2f, WhenConvertingToSvg_ThenExpectSvgPathTag) {
    auto points = create_stanford_bunny_point_list < float > ();

    Transform44f transform = translate(Vector2f(100.f, 100.f));

    size_t point_count = points.size();
    Polygon2f p(transform, &points[0], point_count);

    std::string svg_string = p.convert_to_svg(800, 600);

    EXPECT_THAT(svg_string, testing::HasSubstr("<path"));
    EXPECT_THAT(svg_string, testing::HasSubstr(" Z\""));
    EXPECT_THAT(svg_string, testing::HasSubstr(">"));
    EXPECT_THAT(svg_string, testing::HasSubstr("115.260"));
}

TEST(Polygon2f, AssumePlausibleNormal) {
    std::vector<Point2f> points = create_stanford_bunny_point_list < float > ();

    auto transform = translate(Vector2f{400.f - 50.f, 300.f - 50.f});
    auto point_count = points.size();
    auto polygon = make_reference_counted<Polygon2f>(transform, &points[0], point_count);

    Point2f ray_origin{200.f, 500.f};
    Vector2f ray_direction{1.f, -1.f};
    ray_direction.normalize();

    Ray2f ray{ray_origin, ray_direction, 0.f, 10000.f};

    // Act
    MediumEvent2f its;
    auto hit = polygon->intersect(ray, its);

    // Assert
    EXPECT_TRUE(hit);
    EXPECT_GE(its.p.x(), 100.f);
    EXPECT_LE(its.geo_frame.normal.x(), 0.f);
    EXPECT_GE(its.geo_frame.normal.y(), 0.f);
}

TEST(Polygon2f, GivenARotateCube_WhenComputingIntersection_ThenAssumePlausibeNormals) {
    // Arrange
    std::vector<Point2f> points = create_square(100.f);

    auto transform = translate(Vector2f{400.f, 400.f}) * rotate_z(degree_to_radian(-45.f));
    auto polygon = make_reference_counted<Polygon2f>(transform, &points[0], points.size());

    Point2f ray_origin{200.f, 600.f};
    Vector2f ray_direction{1.f, -1.f};
    ray_direction.normalize();

    Ray2f ray{ray_origin, ray_direction, 0.f, 10000.f};

    // Act
    MediumEvent2f its;
    auto hit = polygon->intersect(ray, its);

    // Assert
    EXPECT_TRUE(hit);
    EXPECT_THAT(its.geo_frame.normal.x(), testing::FloatEq(-sqrt(.5f)));
    EXPECT_THAT(its.geo_frame.normal.y(), testing::FloatEq(sqrt(.5f)));
}

TEST(Polygon2f, GivenScaledPolygonCube_WhenRayIntersectsPolygon_AssumeProperDistance) {
    // Arrange
    std::vector<Point2f> points = create_square(10.f);

    auto transform = translate<float>(Vector2f{400.f, 400.f}) * scale<float>(20);
    auto polygon = make_reference_counted<Polygon2f>(transform, &points[0], points.size());

    Point2f ray_origin{0.f, 400.f};
    Vector2f ray_direction{1.f, 0.f};
    ray_direction.normalize();

    Ray2f ray{ray_origin, ray_direction, 0.f, 10000.f};

    // Act
    MediumEvent2f its;
    auto hit = polygon->intersect(ray, its);

    // Assert
    EXPECT_TRUE(hit);
    EXPECT_THAT(its.t, testing::FloatEq(300.f));
}

TEST(Polygon2f, IfWindingOrderOfProvidedPointsIsIncorrect_ThenThrowException) {
    // Arrange
    std::vector<Point2f> points = create_square(10.f);
    std::reverse(points.begin(), points.end());

    auto transform = translate<float>(Vector2f{400.f, 400.f}) * scale<float>(20);

    EXPECT_THROW(make_reference_counted<Polygon2f>(transform, &points[0], points.size()), std::runtime_error);
}

TEST(Polygon2f, GivenInvalidPoints_WhenConstructionPolygon_ThenRuntimeException) {
    // Arrange
    std::vector<Point2f> points = {
            {0.f,                                    0.f},
            {1.f,                                    0.f},
            {std::numeric_limits<float>::infinity(), 1.f}
    };

    auto transform = identity<float>();

    // Act
    EXPECT_THROW((Polygon2f{transform, &points[0], 3}),
                 std::runtime_error);
}