/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/property_set.h"

#include "gmock/gmock.h"

using namespace flatland;

TEST(PropertySet, Vector2f) {
    PropertySet ps;

    auto v = Vector2f(1.f, 2.f);
    ps.add_property("v", v);

    EXPECT_THAT(ps.get_property<Vector2f>("v"), v);
}

TEST(PropertySet, Vector2d) {
    PropertySet ps;

    auto v = Vector2d(1.0, 2.0);
    ps.add_property("v", v);

    EXPECT_THAT(ps.get_property<Vector2d>("v"), v);
}

TEST(PropertySet, Color3f) {
    PropertySet ps;

    auto c = Color3f(1.f, 1.f, 1.f);
    ps.add_property("c", c);

    EXPECT_THAT(ps.get_property<Color3f>("c"), c);
}

TEST(PropertySet, ObjectProperty) {
    PropertySet ps;
    ps.add_property("obj", make_reference_counted<Object>());

    EXPECT_THAT(ps.get_property<ReferenceCounted<Object>>("obj")->to_string(), "A object. Implement to_string method to improve description.");
}

TEST(PropertySet, MultipleObjects) {
    PropertySet ps;
    ps.add_property("obj1", make_reference_counted<Object>());
    ps.add_property("obj2", make_reference_counted<Object>());
    ps.add_property("obj3", make_reference_counted<Object>());

    EXPECT_THAT(ps.get_property<ReferenceCounted<Object>>("obj1")->to_string(), "A object. Implement to_string method to improve description.");
    EXPECT_THAT(ps.get_property<ReferenceCounted<Object>>("obj2")->to_string(), "A object. Implement to_string method to improve description.");
    EXPECT_THAT(ps.get_property<ReferenceCounted<Object>>("obj3")->to_string(), "A object. Implement to_string method to improve description.");
}
