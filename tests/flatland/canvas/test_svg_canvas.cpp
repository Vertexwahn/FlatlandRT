/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/canvas/svg_canvas.h"
#include "flatland/scene/load_scene.h"
#include "flatland/math/refraction.h"
#include "flatland/scene/shape/disk.h"
#include "flatland/scene/shape/rectangle.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(SvgCanvas, GivenASvgCanvas_WhenConvertingToString_ThenUTF8EncodingHeader) {
	// Arrange
    SvgCanvas2f sc{800, 600};

	// Act
    auto output = sc.toString();

    // Assert
    auto utf8Encoding = R"(<?xml version="1.0" encoding="UTF-8"?>)";
	EXPECT_THAT(output, ::testing::HasSubstr(utf8Encoding));
}

TEST(SvgCanvas, GivenASvgCanvas_WhenConvertingToString_ThenSvgTags) {
    // Arrange
	SvgCanvas2f sc{800, 600};

    // Act
    auto output = sc.toString();

	EXPECT_THAT(output, ::testing::HasSubstr("<svg"));
	EXPECT_THAT(output, ::testing::HasSubstr("</svg>"));
}

TEST(SvgCanvas, GivenASvgCanvasWithLabel_WhenConvertingToString_ThenSvgTextTag) {
    // Arrange
    SvgCanvas2f sc{800, 600};
    Label2f text{Point2f(0.0f, 0.0f), "Text"};
    sc.add(text);

    // Act
    auto output = sc.toString();

    // Assert
    EXPECT_THAT(output, ::testing::HasSubstr("<text"));
    EXPECT_THAT(output, ::testing::HasSubstr("</text>"));
}

TEST(SvgCanvas, GivenASvgCanvasWithARectangle_WhenConvertingToString_ThenSvgRectOutput) {
    // Arrange
    SvgCanvas2f sc{800, 600};

    auto width = 100.0f;
    auto height = 100.0f;
    auto transform = translate<float>(250.0f, 250.0f);
    Rectangle2f rect{transform, width, height};
    sc.add(&rect);

    // Act
    auto output = sc.toString();

    // Assert
    EXPECT_THAT(output, ::testing::HasSubstr("<rect"));
    EXPECT_THAT(output, ::testing::HasSubstr("width=\"100\""));
    EXPECT_THAT(output, ::testing::HasSubstr("height=\"100\""));
    EXPECT_THAT(output, ::testing::HasSubstr("x=\"200\""));
    EXPECT_THAT(output, ::testing::HasSubstr("y=\"300\""));
}

TEST(SvgCanvas, GivenASvgCanvasWithADisk2f_WhenConvertingToString_ThenSvgCircleOutput) {
    // Arrange
	SvgCanvas2f sc{800, 600};
	Disk2f c{translate<float>(Point2f{100.0f,100.0f}), 3.0f};
	sc.add(&c);

	// Act
	auto output = sc.toString();

	// Assert
	EXPECT_THAT(sc.toString(), ::testing::HasSubstr("circle"));
}

TEST(SvgCanvas, When_RectangleWithRedMaterial_Then_ExpectRectangleWithRedFillAndBoderColor) {
    // Arrange
    SvgCanvas2f sc{800, 600};

    auto transform = translate(250.0f, 250.0f);
    Rectangle2f rect{transform, 100.f, 100.0f};

    ReferenceCounted<Material> blueMaterial = makeReferenceCounted<Material>();
	blueMaterial->setStrokeColor(Color3f{0.0f, 0.682f, 0.937f});
	blueMaterial->setStrokeWidth(3.0f);
	blueMaterial->setFillColor(Color3f{1.0f, 1.0f, 1.0f});
    rect.setMaterial(blueMaterial);

    // Act
    sc.add(&rect);

    // Assert
    EXPECT_THAT(sc.toString(), ::testing::HasSubstr("width=\"100\""));
    EXPECT_THAT(sc.toString(), ::testing::HasSubstr("height=\"100\""));
    EXPECT_THAT(sc.toString(), ::testing::HasSubstr("stroke:rgb(0,173.91,238.935)"));
    EXPECT_THAT(sc.toString(), ::testing::HasSubstr("fill:rgb(255,255,255)"));
    EXPECT_THAT(sc.toString(), ::testing::HasSubstr("stroke-width:3"));
}

TEST(SvgCanvas, When_DiskWithRedMaterial_Then_ExpectCicleWithRedFillAndBoderColor) {
    // Arrange
    SvgCanvas2f sc{800, 600};

    ReferenceCounted<Material> blueMaterial = makeReferenceCounted<Material>();
	blueMaterial->setStrokeColor(Color3f(0.0f, 0.682f, 0.937f));
	blueMaterial->setStrokeWidth(3.0f);
	blueMaterial->setFillColor(Color3f(1.0f, 1.0f, 1.0f));

    Disk2f c(translate(Vector2f{200.0f, 200.0f}), 50.0f);
    c.setMaterial(blueMaterial);
    sc.add(&c);

    // Act
    sc.add(&c);

    // Assert
    EXPECT_THAT(sc.toString(), ::testing::HasSubstr("r=\"50\""));
    EXPECT_THAT(sc.toString(), ::testing::HasSubstr("stroke:rgb(0,173.91,238.935)"));
    EXPECT_THAT(sc.toString(), ::testing::HasSubstr("fill:rgb(255,255,255)"));
    EXPECT_THAT(sc.toString(), ::testing::HasSubstr("stroke-width:3"));
}

TEST(SvgCanvas, WhenTryToSaveToAnInvalidLocation_ThenRuntimeException) {
    // Arrange
    SvgCanvas2f sc{800, 600};

    // Cannot write outside of test dirs
    EXPECT_THROW(sc.store("/home/newton/Ray2CircleIntersection8.svg"), std::runtime_error);
}

TEST(SvgCanvas, When_ShapeHasBlueMaterial_ExpectInSvgBlueColor) {
    // Todo: Read canvas size from Scene.xml
    SvgCanvas2f sc{600, 700};

    Vector2f position(300.0f, 325.0f);
    Rectangle2f rect(translate(position), 400.f, 250.0f);

    auto rectColor = Color3f(0.0f, 0.682f, 0.937f);
    ReferenceCounted<Material> blueMaterial = makeReferenceCounted<Material>();
    blueMaterial->setStrokeColor(rectColor);
    blueMaterial->setStrokeWidth(3.0f);
    blueMaterial->setFillColor(Color3f(1.0f, 1.0f, 1.0f));
    rect.setMaterial(blueMaterial);
    sc.add(&rect);

    // todo: Materials seem not to work for spheres - add unit test
    // Disk2f disk(Vector2f(300.0f, 200.0f), 50.0f);
    // disk.setMaterial(blueMaterial.get());
    // sc.add(&disk);

    Point2f rayOrigin(100.0f, 600.0f);

    Vector2f rayDirection(1.0f, -1.0f); // shoot away - expected is no hit
    rayDirection.normalize();

    Ray2f ray(rayOrigin, rayDirection, 0.0f, 100.0f);

    MediumEvent2f its;
    rect.intersect(ray, its);
    ray.max_t = its.t;
    sc.add(ray);

    // compute refraction
    Vector2f refractedDirection;
    Vector2f incidentVector = Vector2f(-1.0f, 1.0f);
    incidentVector.normalize();
    bool result = refract(incidentVector, its.n, 1.0f / 1.6f, refractedDirection);

    std::cout << "its.n = " << its.n << std::endl;
    std::cout << "result = " << result << std::endl;
    std::cout << "incidentVector = " << incidentVector << std::endl;
    std::cout << refractedDirection.norm() << std::endl;

    Ray2f refractedRay(its.p + refractedDirection * 0.01f, refractedDirection, 0.0f, 200.0f);

    sc.add(refractedRay);

    // todo: add test for text font size...
    sc.add(Label2f{ray.origin + Vector2f(0.5f, 0.0f), "Incident vector"});
    sc.add(Label2f{refractedRay(refractedRay.max_t), "Refracted vector"});

    EXPECT_THAT(sc.toString(), ::testing::HasSubstr("rgb(0,173.91,238.935)"));
}

TEST(SvgCanvas, When_ShapeHasBlueMaterial_ExpectInSvgBlueColor2)
{
    // how to connect svgcanvas and scene?
    auto scene = loadScene("scenes/rectangle.flatland.xml");

    SvgCanvas2f sc{scene->getCamera()->getFilm().getSize()};

    for(auto shape : scene->getShapes()) {
        sc.add(shape.get());
    }

    Point2f rayOrigin(100.0f, 600.0f);

    Vector2f rayDirection(1.0f, -1.0f); // shoot away - expected is no hit
    rayDirection.normalize();

    Ray2f ray(rayOrigin, rayDirection, 0.0f, 100.0f);

    MediumEvent2f its;
    scene->intersect(ray, its);
    ray.max_t = its.t;
    sc.add(ray);

    // compute refraction
    Vector2f refractedDirection;
    Vector2f incidentVector = Vector2f(-1.0f, 1.0f);
    incidentVector.normalize();
    bool result = refract(incidentVector, its.n, 1.0f / 1.6f, refractedDirection);

    std::cout << "its.n = " << its.n << std::endl;
    std::cout << "result = " << result << std::endl;
    std::cout << "incidentVector = " << incidentVector << std::endl;
    std::cout << refractedDirection.norm() << std::endl;

    Ray2f refractedRay(its.p + refractedDirection * 0.01f, refractedDirection, 0.0f, 200.0f);

    sc.add(refractedRay);

    // todo: add test for text font size...
    sc.add(Label2f{ray.origin + Vector2f(0.5f, 0.0f), "Incident vector"});
    sc.add(Label2f{refractedRay(refractedRay.max_t), "Refracted vector"});

    EXPECT_THAT(sc.toString(), ::testing::HasSubstr("rgb(0,173.91,238.935)"));
}

TEST(SvgCanvas, GivenARectangle_WhenSceneFileMaterialDefineStrokeWidth3_ThenExpectSvgStrokeWidth3) {
    auto scene = loadScene("scenes/rectangle.flatland.xml");

    SvgCanvas2f sc{scene->getCamera()->getFilm().getSize()};

    for (auto shape : scene->getShapes()) {
        sc.add(shape.get());
    }

    EXPECT_THAT(sc.toString(), ::testing::HasSubstr("stroke-width:3"));
}

TEST(SvgCanvas, GivenAPolygon_WhenSceneFileMaterialDefineStrokeWidth3_ThenExpectSvgStrokeWidth3) {
    auto scene = loadScene("scenes/bunny.flatland.xml");

    SvgCanvas2f sc{scene->getCamera()->getFilm().getSize()};

    for (auto shape : scene->getShapes()) {
        sc.add(shape.get());
    }

    EXPECT_THAT(sc.toString(), ::testing::HasSubstr("stroke-width:3"));
}

TEST(SvgCanvas, GivenADisk_WhenSceneFileMaterialDefineStrokeWidth3_ThenExpectSvgStrokeWidth3) {
    auto scene = loadScene("scenes/disk.flatland.xml");

    SvgCanvas2f sc{scene->getCamera()->getFilm().getSize()};

    for (auto shape : scene->getShapes()) {
        sc.add(shape.get());
    }

    EXPECT_THAT(sc.toString(), ::testing::HasSubstr("stroke-width:3"));
}

TEST(SvgCanvas, GivenCanvasWithShapes_WhenCountingShapes_ThenExpectMoreThan1Shape) {
    // Arrange
    auto scene = loadScene("scenes/mirror.flatland.xml");

    SvgCanvas2f sc{scene->getCamera()->getFilm().getSize()};

    for (auto shape : scene->getShapes()) {
        sc.add(shape.get());
    }

    // Act
    auto count = sc.getShapeCount();

    // Assert
    EXPECT_THAT(count, ::testing::Ge(1u));
}