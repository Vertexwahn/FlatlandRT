/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/property_set_formatter.hpp"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(fmtFormatter, ColorRGB3f) {
    ColorRGB3f c{1.f, 0.f, 1.f};
    std::string str = fmt::format("{}", c);
    EXPECT_THAT(str, "(1, 0, 1)");
}

TEST(fmtFormatter, RefCountedObject) {
    ReferenceCounted<Object> obj = make_reference_counted<Object>();
    std::string str = fmt::format("{}", obj);
    EXPECT_THAT(str, "An object. Implement the to_string method to improve this description.");
}

TEST(fmtFormatter, Point2f) {
    Point2f p{1.f, 2.f};
    std::string str = fmt::format("{}", p);
    EXPECT_THAT(str, "(1, 2)");
}

TEST(fmtFormatter, Point3f) {
    Point3f p{1.f, 2.f, 3.f};
    std::string str = fmt::format("{}", p);
    EXPECT_THAT(str, "(1, 2, 3)");
}

TEST(fmtFormatter, Point2d) {
    Point2d p{1.0, 2.0};
    std::string str = fmt::format("{}", p);
    EXPECT_THAT(str, "(1, 2)");
}

TEST(fmtFormatter, Transform44f) {
    Transform44f t{identity<float>()};
    std::string str = fmt::format("{}", t);
    EXPECT_THAT(str, "(1)");
}

TEST(fmtFormatter, Vector2f) {
    Vector2f v{1.f, 2.f};
    std::string str = fmt::format("{}", v);
    EXPECT_THAT(str, "(1, 2)");
}

TEST(fmtFormatter, Vector2d) {
    Vector2d v{1.0, 2.0};
    std::string str = fmt::format("{}", v);
    EXPECT_THAT(str, "(1, 2)");
}

TEST(fmtFormatter, Vector3f) {
    Vector3f v{1.f, 2.f, 3.f};
    std::string str = fmt::format("{}", v);
    EXPECT_THAT(str, "(1, 2, 3)");
}
