/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/math/util.h"
#include "flatland/scene/shape/rectangle.h"

using namespace Flatland;

#include <catch2/catch.hpp>

TEST_CASE("Rectangle2f", "[GivenAPreferedSizeForARectangle_WhenInitializing_ThenExpectedTransformHeightWidthMinAndMax]") {
    Point2f p{100.0f, 100.0f};
    Vector2f t{50.0f, 50.0f};
    Point2f ps = p + t;
    auto ps2 = p + t;
    REQUIRE(ps == Point2f{150.0f, 150.0f});
    REQUIRE(ps2 == Point2f{150.0f, 150.0f});

    Point2f minimum{50.0f, 200.0f};
    Point2f maximum{200.0f, 300.0f};
    auto width = maximum.x() - minimum.x();
    auto height = maximum.y() - minimum.y();

    REQUIRE(width == 150.0f);
    REQUIRE(height == 100.0f);

    //auto translation = minimum + Vector2f{width/2.0f, height/2.0f};
    Point2f translation = minimum + Vector2f{width/2.0f, height/2.0f};
    std::cout << "mh" << std::endl;

    const std::type_info& r1 = typeid(translation);
    std::cout << '\n' << "std::cout<<myint has type : " << r1.name() << '\n';

    REQUIRE(Vector2f{width/2.0f, height/2.0f} == Vector2f{75.0f, 50.0f});

    PropertySet pset;
    pset.addProperty("width", width);
    pset.addProperty("height", height);
    pset.addProperty("transform", translate(translation.x(), translation.y()));
    Rectangle2f rect{pset};

    REQUIRE(translation.x() == 125.0f);

    Matrix44f expectedMatrix;
    expectedMatrix << 1.0f, 0.0f, 0.0f, translation.x(),
            0.0f, 1.0f, 0.0f, translation.y(),
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f;

    REQUIRE(rect.getTransform().getMatrix() == expectedMatrix);
}