/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/scene/load_scene.h"
#include "flatland/scene/shape/shape.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(Interection2f, GivenIntersectionPointAndDistanceAndNormalWhenInitializing_ThenInitializedIntersection) {
    Point2f pointOfIntersection{10.0f, 10.0f};
    auto distance = 3.0f;
    Normal2f normal{0.0f, 1.0f};

    Intersection2f its{pointOfIntersection, distance, normal};

    EXPECT_THAT(its.p, pointOfIntersection);
    EXPECT_THAT(its.t, distance);
    EXPECT_THAT(its.n, normal);
}

class DummyShape : public Shape2f {
public:
    DummyShape(const Transform44f& transform) : Shape2f(transform) {
    }

    virtual bool intersect(const Ray2f &ray, MediumEvent2f &its) const override { return  false; }

    virtual std::string convertToSvg(const int svgCanvasWidth, const int svgCanvasHeight) const override {return "";};

    Transform44f& getTransform() {
        return transform_;
    }
};

TEST(Shape, GivenADummyShape_WhenInitializedWithTranslation_ExpectTranslationTransform) {
    auto t = translate(Vector2f{10.0f, 20.0f});
    DummyShape s{t};

    EXPECT_EQ(s.getTransform().getMatrix()(0,3), 10.0f);
    EXPECT_EQ(s.getTransform().getMatrix()(1,3), 20.0f);
}

TEST(MediumEvent, GivenAShapeWithAMaterial_WhenRayIntersectsDisk_ThenMediumEventWithRedMaterial) {
    // Arrange
    auto scene = loadScene("scenes/disk.flatland.xml");

    // Act
    Ray2f ray = scene->getCamera()->generateRay();
    MediumEvent2f me;
    bool hit = scene->intersect(ray, me);

    EXPECT_TRUE(hit);

    ASSERT_NE(me.material, nullptr);
    float indexOfRefraction = me.material->getRefractionIndex();

    // Assert
    EXPECT_THAT(indexOfRefraction, 1.0f);
    EXPECT_THAT(me.material->getInterInteractionType(), eInterfaceInteraction::specularTransmission);
}