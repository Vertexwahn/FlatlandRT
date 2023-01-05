/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/property_set.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(PropertyDoesNotExistException, ErrorMessageTest) {
    PropertyDoesNotExistException my_exception("foobar");
    EXPECT_THAT(std::string(my_exception.what()), testing::Eq("Property with name 'foobar' does not exist"));
}

TEST(PropertyDoesAlreadyExistException, TestErrorMessage) {
    PropertyDoesAlreadyExistException ex("foobar", 1);
    EXPECT_THAT(std::string(ex.what()),
                testing::Eq("Property with name 'foobar' does already exist and its value is '1'"));
}

TEST(PropertySet, TestBasicTypes) {
    using VariantType = std::variant<
            int,
            float,
            double,
            char,
            std::string,
            bool>;

    using PropertySet = PropertySetType<VariantType>;

    PropertySet ps;

    ps.add_property("a", 1);
    ps.add_property("b", 1.f);
    ps.add_property("c", 1.0);
    ps.add_property("d", 'a');
    ps.add_property("e", std::string("Hello World!"));
    ps.add_property("f", true);
    ps.add_property("g", false);
    ps.add_property("longer_variable_name", std::string("some_string"));

    EXPECT_THAT(ps.get_property<int>("a"), 1);
    EXPECT_THAT(ps.get_property<int>("a"), 1);
    EXPECT_THAT(ps.get_property<float>("b"), 1.f);
    EXPECT_THAT(ps.get_property<double>("c"), 1.0);
    EXPECT_THAT(ps.get_property<char>("d"), 'a');
    EXPECT_THAT(ps.get_property<std::string>("e"), "Hello World!");
    EXPECT_THAT(ps.get_property<bool>("f"), true);
    EXPECT_THAT(ps.get_property<bool>("g"), false);
    EXPECT_THAT(ps.get_property<std::string>("longer_variable_name"), "some_string");
}

TEST(PropertySet, When_PropertyDoesNotExist_Then_RaiseException) {
    using VariantType = std::variant<int>;
    using PropertySet = PropertySetType<VariantType>;

    PropertySet ps;
    EXPECT_THROW(ps.get_property<int>("a"), PropertyDoesNotExistException);
}

TEST(PropertySet, WhenPropertyExist_ThenReturnTrue) {
    using VariantType = std::variant<int>;
    using PropertySet = PropertySetType<VariantType>;

    PropertySet ps;

    ps.add_property("a", 1);

    EXPECT_TRUE(ps.has_property("a"));
}

TEST(PropertySet, WhenPropertyDoesNotExist_ThenReturnFalse) {
    using VariantType = std::variant<int>;
    using PropertySet = PropertySetType<VariantType>;

    PropertySet ps;
    EXPECT_FALSE(ps.has_property("b"));
}

TEST(PropertySet, WhenPropertyDoesNotExist_Then_ReturnDefaultValue) {
    using VariantType = std::variant<int>;
    using PropertySet = PropertySetType<VariantType>;

    PropertySet ps;
    EXPECT_THAT(ps.get_property("notExistingProperty", 42), testing::Eq(42));
}

TEST(PropertySet, CopyCtor) {
    using VariantType = std::variant<int>;
    using PropertySet = PropertySetType<VariantType>;

    PropertySet ps1;
    ps1.add_property("a", 1);

    PropertySet ps2 = ps1;

    EXPECT_THAT(ps2.get_property<int>("a"), testing::Eq(1));
}

TEST(PropertySet, WhenSamePropertyIsAddedTwice_ThenThrowExceptionAndExpectProperErrorMessage) {
    using VariantType = std::variant<int>;
    using PropertySet = PropertySetType<VariantType>;

    PropertySet ps;
    ps.add_property("a", 1);

    EXPECT_THROW(ps.add_property("a", 2), PropertyDoesAlreadyExistException);
}

TEST(PropertySet, WhenGetIsCalledWithTheWrongType_ThenThrowException) {
    using VariantType = std::variant<int, float>;
    using PropertySet = PropertySetType<VariantType>;

    PropertySet ps;
    ps.add_property("a", 1);

    EXPECT_THROW(ps.get_property<float>("a"), PropertyWithWrongValueTypeRequested);
}

TEST(PropertySet, set_property) {
    using VariantType = std::variant<int, float>;
    using PropertySet = PropertySetType<VariantType>;

    PropertySet ps;
    ps.add_property("a", 1);
    ps.set_property("a", 2);

    EXPECT_THAT(ps.get_property<int>("a"), 2);
}

TEST(PropertySet, WhenTryingToSetAPropertyThatDoesNotExist_ThenThrowException) {
    using VariantType = std::variant<int, float>;
    using PropertySet = PropertySetType<VariantType>;

    PropertySet ps;
    EXPECT_THROW(ps.set_property("a", 1), PropertyDoesNotExistException);
}

TEST(PropertySet, WhenTryingToSetAPropertyWithAValueOfAnInvalidType_ThenThrowException) {
    using VariantType = std::variant<int, float>;
    using PropertySet = PropertySetType<VariantType>;

    PropertySet ps;
    ps.add_property("a", 1);
    EXPECT_THROW(ps.set_property("a", 1.f), PropertyWithWrongValueTypeRequested);
}
