/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/scene/load_scene.hpp"

#include "math/refract.hpp"
#include "flatland/rendering/canvas/svg_canvas.hpp"
#include "flatland/rendering/shape/disk.hpp"
#include "flatland/rendering/shape/rectangle.hpp"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(SvgCanvas, GivenASvgCanvas_WhenConvertingToString_ThenUTF8EncodingHeader) {
    // Arrange
    SvgCanvas2f sc{800, 600};

    // Act
    auto output = sc.to_string();

    // Assert
    auto utf8Encoding = R"(<?xml version="1.0" encoding="UTF-8"?>)";
    EXPECT_THAT(output, testing::HasSubstr(utf8Encoding));
}

TEST(SvgCanvas, GivenASvgCanvas_WhenConvertingToString_ThenSvgTags) {
    // Arrange
    SvgCanvas2f sc{800, 600};

    // Act
    auto output = sc.to_string();

    EXPECT_THAT(output, testing::HasSubstr("<svg"));
    EXPECT_THAT(output, testing::HasSubstr("</svg>"));
}

TEST(SvgCanvas, GivenASvgCanvasWithLabel_WhenConvertingToString_ThenSvgTextTag) {
    // Arrange
    SvgCanvas2f sc{800, 600};
    Label2f text{Point2f(0.f, 0.f), "Text"};
    sc.add(text);

    // Act
    auto output = sc.to_string();

    // Assert
    EXPECT_THAT(output, testing::HasSubstr("<text"));
    EXPECT_THAT(output, testing::HasSubstr("</text>"));
}

/*
TEST(SvgCanvas, GivenASvgCanvasWithARectangle_WhenConvertingToString_ThenSvgRectOutput) {
    // Arrange
    SvgCanvas2f sc{800, 600};

    auto width = 100.f;
    auto height = 100.f;
    auto transform = translate<float>(250.f, 250.f);

    PropertySet ps;
    ps.addProperty("width", width);
    ps.addProperty(name:)

    Rectangle2f rect{transform, width, height};
    sc.add(&rect);

    // Act
    auto output = sc.to_string();

    // Assert
    EXPECT_THAT(output, testing::HasSubstr("<rect"));
    EXPECT_THAT(output, testing::HasSubstr("width=\"100\""));
    EXPECT_THAT(output, testing::HasSubstr("height=\"100\""));
    EXPECT_THAT(output, testing::HasSubstr("x=\"200\""));
    EXPECT_THAT(output, testing::HasSubstr("y=\"300\""));
}
*/

TEST(SvgCanvas, GivenASvgCanvasWithADisk2f_WhenConvertingToString_ThenSvgCircleOutput) {
    // Arrange
    SvgCanvas2f sc{800, 600};

    PropertySet ps;
    ps.add_property("radius", 3.f);
    ps.add_property("transform", translate(Vector2f{100.f, 100.f}));

    Disk2f c{ps};
    sc.add(&c);

    // Act
    auto output = sc.to_string();

    // Assert
    EXPECT_THAT(sc.to_string(), testing::HasSubstr("circle"));
}

TEST(SvgCanvas, GivenRectangle_WhenAssigningMaterial_ExpectSpecificFillAndBoderColor) {
    // Arrange
    SvgCanvas2f sc{800, 600};

    auto transform = translate(250.f, 250.f);

    PropertySet ps;
    ps.add_property("width", 100.f);
    ps.add_property("height", 100.f);
    ps.add_property("transform", transform);

    Rectangle2f rect{ps};

    // Act
    PropertySet ps_mat;
    ps_mat.add_property("stroke_color", ColorRGB3f{0.f, 0.682f, 0.937f});
    ps_mat.add_property("stroke_width", 3.f);
    ps_mat.add_property("fill_color", ColorRGB3f{1.f, 1.f, 1.f});
    ReferenceCounted<SvgMaterial> blue_material = make_reference_counted<SvgMaterial>(ps_mat);
    rect.set_bsdf(blue_material);

    sc.add(&rect);

    // Assert
    EXPECT_THAT(sc.to_string(), testing::HasSubstr("stroke:rgb(0,173.91,238.935)"));
    EXPECT_THAT(sc.to_string(), testing::HasSubstr("fill:rgb(255,255,255)"));
    EXPECT_THAT(sc.to_string(), testing::HasSubstr("stroke-width:3"));
}

TEST(SvgCanvas, When_DiskWithRedMaterial_Then_ExpectCicleWithRedFillAndBoderColor) {
    // Arrange
    SvgCanvas2f sc{800, 600};

    PropertySet ps_mat;
    ps_mat.add_property("stroke_color", ColorRGB3f{0.f, 0.682f, 0.937f});
    ps_mat.add_property("stroke_width", 3.f);
    ps_mat.add_property("fill_color", ColorRGB3f{1.f, 1.f, 1.f});
    ReferenceCounted<SvgMaterial> blue_material = make_reference_counted<SvgMaterial>(ps_mat);

    PropertySet ps;
    ps.add_property("radius", 50.f);
    ps.add_property("transform", translate(Vector2f{200.f, 200.f}));

    Disk2f c{ps};
    c.set_bsdf(blue_material);
    sc.add(&c);

    // Act
    sc.add(&c);

    // Assert
    EXPECT_THAT(sc.to_string(), testing::HasSubstr("r=\"50\""));
    EXPECT_THAT(sc.to_string(), testing::HasSubstr("stroke:rgb(0,173.91,238.935)"));
    EXPECT_THAT(sc.to_string(), testing::HasSubstr("fill:rgb(255,255,255)"));
    EXPECT_THAT(sc.to_string(), testing::HasSubstr("stroke-width:3"));
}

TEST(SvgCanvas, WhenTryToSaveToAnInvalidLocation_ThenRuntimeException) {
    // Arrange
    SvgCanvas2f sc{800, 600};

    // Cannot write outside of test dirs
    EXPECT_THROW(sc.store("/home/not_existing_user/Ray2CircleIntersection8.svg"), std::runtime_error);
}

TEST(SvgCanvas, When_ShapeHasblue_material_ExpectInSvgBlueColor) {
    // Todo: Read canvas size from Scene.xml
    SvgCanvas2f sc{600, 700};

    Vector2f position(300.f, 325.f);

    PropertySet ps;
    ps.add_property("width", 400.f);
    ps.add_property("height", 250.f);
    ps.add_property("transform", translate(position));

    Rectangle2f rect(ps);

    PropertySet ps_mat;
    ps_mat.add_property("stroke_color", ColorRGB3f{0.f, 0.682f, 0.937f});
    ps_mat.add_property("stroke_width", 3.f);
    ps_mat.add_property("fill_color", ColorRGB3f{1.f, 1.f, 1.f});
    ReferenceCounted<SvgMaterial> blue_material = make_reference_counted<SvgMaterial>(ps_mat);

    rect.set_bsdf(blue_material);
    sc.add(&rect);

    // todo: Materials seem not to work for spheres - add unit test
    // Disk2f disk(Vector2f(300.f, 200.f), 50.f);
    // disk.set_material(blue_material.get());
    // sc.add(&disk);

    Point2f ray_origin(100.f, 600.f);

    Vector2f ray_direction(1.f, -1.f); // shoot away - expected is no hit
    ray_direction.normalize();

    Ray2f ray(ray_origin, ray_direction, 0.f, 100.f);

    MediumEvent2f its;
    rect.intersect(ray, its);
    ray.max_t = its.t;
    sc.add(ray);

    // compute refraction
    Vector2f refracted_direction;
    Vector2f incident_vector = Vector2f(-1.f, 1.f);
    incident_vector.normalize();
    bool result = refract(incident_vector, its.geo_frame.n, 1.f / 1.6f, refracted_direction);
    EXPECT_THAT(result, true);

    Ray2f refracted_ray(its.p + refracted_direction * 0.01f, refracted_direction, 0.f, 200.f);

    sc.add(refracted_ray);

    // todo: add test for text font size...
    sc.add(Label2f{ray.origin + Vector2f(.5f, 0.f), "Incident vector"});
    sc.add(Label2f{refracted_ray(refracted_ray.max_t), "Refracted vector"});

    EXPECT_THAT(sc.to_string(), testing::HasSubstr("rgb(0,173.91,238.935)"));
}

TEST(SvgCanvas, When_ShapeHasblue_material_ExpectInSvgBlueColor2) {
    auto scene = load_scene2f("flatland/scenes/rectangle.flatland.xml");

    SvgCanvas2f sc{scene->sensor()->film()->size()};

    for (auto shape: scene->shapes()) {
        sc.add(shape.get());
    }

    Point2f ray_origin(100.f, 600.f);

    Vector2f ray_direction(1.f, -1.f); // shoot away - expected is no hit
    ray_direction.normalize();

    Ray2f ray(ray_origin, ray_direction, 0.f, 100.f);

    MediumEvent2f its;
    scene->intersect(ray, its);
    ray.max_t = its.t;
    sc.add(ray);

    // compute refraction
    Vector2f refracted_direction;
    Vector2f incidentVector = Vector2f(-1.f, 1.f);
    incidentVector.normalize();

    bool result = refract(incidentVector, its.geo_frame.n, 1.f / 1.6f, refracted_direction);
    EXPECT_TRUE(result);

    Ray2f refracted_ray(its.p + refracted_direction * 0.01f, refracted_direction, 0.f, 200.f);

    sc.add(refracted_ray);
    sc.add(Label2f{ray.origin + Vector2f(.5f, 0.f), "Incident vector"});
    sc.add(Label2f{refracted_ray(refracted_ray.max_t), "Refracted vector"});

    EXPECT_THAT(sc.to_string(), testing::HasSubstr("rgb(0,173.91,238.935)"));
}

TEST(SvgCanvas, GivenARectangle_WhenSceneFileMaterialDefineStrokeWidth3_ThenExpectSvgStrokeWidth3) {
    auto scene = load_scene2f("flatland/scenes/rectangle.flatland.xml");

    SvgCanvas2f sc{scene->sensor()->film()->size()};

    for (auto shape: scene->shapes()) {
        sc.add(shape.get());
    }

    EXPECT_THAT(sc.to_string(), testing::HasSubstr("stroke-width:3"));
}

TEST(SvgCanvas, GivenAPolygon_WhenSceneFileMaterialDefineStrokeWidth3_ThenExpectSvgStrokeWidth3) {
    auto scene = load_scene2f("flatland/scenes/bunny/bunny.flatland.xml");

    SvgCanvas2f sc{scene->sensor()->film()->size()};

    for (auto shape: scene->shapes()) {
        sc.add(shape.get());
    }

    EXPECT_THAT(sc.to_string(), testing::HasSubstr("stroke-width:3"));
}

TEST(SvgCanvas, GivenADisk_WhenSceneFileMaterialDefineStrokeWidth3_ThenExpectSvgStrokeWidth3) {
    auto scene = load_scene2f("flatland/scenes/disk.flatland.xml");

    SvgCanvas2f sc{scene->sensor()->film()->size()};

    for (auto shape: scene->shapes()) {
        sc.add(shape.get());
    }

    EXPECT_THAT(sc.to_string(), testing::HasSubstr("stroke-width:3"));
}

TEST(SvgCanvas, GivenCanvasWithShapes_WhenCountingShapes_ThenExpectMoreThan1Shape) {
    // Arrange
    auto scene = load_scene2f("flatland/scenes/mirror.flatland.xml");

    SvgCanvas2f sc{scene->sensor()->film()->size()};

    for (auto shape: scene->shapes()) {
        sc.add(shape.get());
    }

    // Act
    auto count = sc.shape_count();

    // Assert
    EXPECT_THAT(count, testing::Ge(1u));
}

TEST(SvgCanvas, bounds) {
    // Arrange
    SvgCanvas2f sc{800, 600};
    AxisAlignedBoundingBox2f aabb{{100.f, 100.f},
                                  {200.f, 200.0f}};

    // Act
    sc.add(aabb);
    auto result = sc.to_string();

    // Assert
    EXPECT_THAT(result, testing::HasSubstr("<rect x=\"100\" y=\"400\" width=\"100\" height=\"100\""));
}
