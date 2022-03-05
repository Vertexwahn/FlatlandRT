/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/math/util.h"
#include "flatland/rendering/scene/shape/rectangle.h"

using namespace flatland;

#include "catch2/catch_test_macros.hpp"

TEST_CASE("Rectangle2f",
          "[GivenApreferredSizeForARectangle_WhenInitializing_ThenExpectedTransformHeightWidthMinAndMax]") {
    Point2f p{100.f, 100.f};
    Vector2f t{50.f, 50.f};
    Point2f ps = p + t;
    auto ps2 = p + t;
    REQUIRE(ps == Point2f{150.f, 150.f});
    REQUIRE(ps2 == Point2f{150.f, 150.f});

    Point2f minimum{50.f, 200.f};
    Point2f maximum{200.f, 300.f};
    auto width = maximum.x() - minimum.x();
    auto height = maximum.y() - minimum.y();

    REQUIRE(width == 150.f);
    REQUIRE(height == 100.f);

    //auto translation = minimum + Vector2f{width/2.f, height/2.f};
    Point2f translation = minimum + Vector2f{width / 2.f, height / 2.f};

    //const std::type_info& r1 = typeid(translation);
    //std::cout << '\n' << "std::cout<<myint has type : " << r1.name() << '\n';

    REQUIRE(Vector2f{width / 2.f, height / 2.f} == Vector2f{75.f, 50.f});

    PropertySet pset;
    pset.add_property("width", width);
    pset.add_property("height", height);
    pset.add_property("transform", translate(translation.x(), translation.y()));
    Rectangle2f rect{pset};

    REQUIRE(translation.x() == 125.f);

    Matrix44f expected_matrix;
    expected_matrix << 1.f, 0.f, 0.f, translation.x(),
            0.f, 1.f, 0.f, translation.y(),
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f;

    REQUIRE(rect.transform().matrix() == expected_matrix);
}