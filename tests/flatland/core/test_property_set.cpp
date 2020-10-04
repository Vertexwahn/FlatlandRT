/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/core/property_set.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(PropertyDoesNotExistException, ErrorMessageTest) {
    PropertyDoesNotExistException ex("foobar");
    EXPECT_THAT(std::string(ex.what()), ::testing::Eq("Property with name 'foobar' does not exist"));
}

TEST(PropertyDoesAlreadyExistException, TestErrorMessage) {
    PropertyDoesAlreadyExistException ex("foobar", 1);
    EXPECT_THAT(std::string(ex.what()),
                ::testing::Eq("Property with name 'foobar' does already exist and its value is '1'"));
}

TEST(PropertySet, TestBasicTypes) {
    PropertySet ps;

    ps.addProperty("a", 1);
    ps.addProperty("b", 1.0f);
    ps.addProperty("c", 1.0);
    ps.addProperty("d", 'a');
    ps.addProperty("e", std::string("Hello World!"));
    ps.addProperty("f", true);
    ps.addProperty("g", false);

    EXPECT_EQ(ps.getProperty<int>("a"), 1);
    EXPECT_EQ(ps.getProperty<int>("a"), 1);
    EXPECT_EQ(ps.getProperty<float>("b"), 1.0f);
    EXPECT_EQ(ps.getProperty<double>("c"), 1.0);
    EXPECT_EQ(ps.getProperty<char>("d"), 'a');
    EXPECT_EQ(ps.getProperty<std::string>("e"), "Hello World!");
    EXPECT_EQ(ps.getProperty<bool>("f"), true);
    EXPECT_EQ(ps.getProperty<bool>("g"), false);
}

TEST(PropertySet, Test_Vector2f) {
    PropertySet ps;

    auto v = Vector2f(1.0f, 2.0f);
    ps.addProperty("v", v);

    EXPECT_EQ(ps.getProperty<Vector2f>("v"), v);
}

TEST(PropertySet, Test_Vector2d) {
    PropertySet ps;

    auto v = Vector2d(1.0, 2.0);
    ps.addProperty("v", v);

    EXPECT_EQ(ps.getProperty<Vector2d>("v"), v);
}

TEST(PropertySet, Test_Color3f) {
    PropertySet ps;

    auto c = Color3f(1.0f, 1.0f, 1.0f);
    ps.addProperty("c", c);

    EXPECT_EQ(ps.getProperty<Color3f>("c"), c);
}

TEST(PropertySet, When_PropertyDoesNotExist_Then_RaiseException) {
    PropertySet ps;
    EXPECT_THROW(ps.getProperty<int>("a"), PropertyDoesNotExistException);
}

TEST(PropertySet, WhenPropertyExist_ThenReturnTrue) {
    PropertySet ps;

    ps.addProperty("a", 1);

    EXPECT_TRUE(ps.hasProperty("a"));
}

TEST(PropertySet, WhenPropertyDoesNotExist_ThenReturnFalse) {
    PropertySet ps;
    EXPECT_FALSE(ps.hasProperty("b"));
}

TEST(PropertySet, WhenPropertyDoesNotExist_Then_ReturnDefaultValue) {
    PropertySet ps;
    EXPECT_THAT(ps.getProperty("notExistingProperty", 42), ::testing::Eq(42));
}

TEST(PropertySet, CopyCtor) {
    PropertySet ps1;
    ps1.addProperty("a", 1);

    PropertySet ps2 = ps1;

    EXPECT_THAT(ps2.getProperty<int>("a"), testing::Eq(1));
}

TEST(PropertySet, WhenSamePropertyIsAddedTwice_ThenThrowExceptionAndExpectProperErrorMessage) {
    PropertySet ps;
    ps.addProperty("a", 1);

    EXPECT_THROW(ps.addProperty("a", 2), PropertyDoesAlreadyExistException);
}

TEST(PropertySet, WhenGetIsCalledWithTheWrongType_ThenThrowException) {
    PropertySet ps;
    ps.addProperty("a", 1);

    EXPECT_THROW(ps.getProperty<float>("a"), PropertyWithWrongValueTypeRequested);
}

// Todo: read this https://www.modernescpp.com/index.php/c-17-avoid-copying-with-std-string-view -> use std::string_view
// https://www.grimm-jaud.de/index.php/blog/c-17-vermeide-kopieren-mit-std-string-view

// Todo: implement operator[] int x = ps["muh"]; -> just implement that stuff that is used
// Todo: what happens if some one intorduce sleep(2 days) will this be detected <- I gues so because of bazel!
// Todo: What happens if someone adds bad code - i.e. if 42 return 43
