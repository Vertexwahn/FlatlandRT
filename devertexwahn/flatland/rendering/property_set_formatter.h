/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_PopertySet_formatter_a272aef3_074e_4168_ae05_64383371db33_h
#define De_Vertexwahn_Flatland_PopertySet_formatter_a272aef3_074e_4168_ae05_64383371db33_h

#include "core/object.h"
#include "core/reference_counted.h"
#include "imaging/color.h"
#include "math/transform.h"
#include "math/util.h"

#include "fmt/format.h"

template<>
struct fmt::formatter<de_vertexwahn::Vector2f> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const de_vertexwahn::Vector2f &v, FormatContext &ctx) {
        return fmt::format_to(ctx.out(), "({}, {})", v.x(), v.y());
    }
};

template<>
struct fmt::formatter<de_vertexwahn::Vector2d> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const de_vertexwahn::Vector2d &v, FormatContext &ctx) {
        return fmt::format_to(ctx.out(), "({}, {})", v.x(), v.y());
    }
};

template<>
struct fmt::formatter<de_vertexwahn::Vector3f> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const de_vertexwahn::Vector3f &v, FormatContext &ctx) {
        return fmt::format_to(ctx.out(), "({}, {}, {})", v.x(), v.y(), v.z());
    }
};

template<>
struct fmt::formatter<de_vertexwahn::Vector3d> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const de_vertexwahn::Vector3f &v, FormatContext &ctx) {
        return fmt::format_to(ctx.out(), "({}, {}, {})", v.x(), v.y(), v.z());
    }
};

template<>
struct fmt::formatter<de_vertexwahn::Point2f> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const de_vertexwahn::Point2f &v, FormatContext &ctx) {
        return fmt::format_to(ctx.out(), "({}, {})", v.x(), v.y());
    }
};

template<>
struct fmt::formatter<de_vertexwahn::Point3f> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const de_vertexwahn::Point3f &v, FormatContext &ctx) {
        return fmt::format_to(ctx.out(), "({}, {}, {})", v.x(), v.y(), v.z());
    }
};

template<>
struct fmt::formatter<de_vertexwahn::Point2d> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const de_vertexwahn::Point2d &v, FormatContext &ctx) {
        return fmt::format_to(ctx.out(), "({}, {})", v.x(), v.y());
    }
};

template<>
struct fmt::formatter<de_vertexwahn::Color3f> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const de_vertexwahn::Color3f &c, FormatContext &ctx) {
        return fmt::format_to(ctx.out(), "({}, {}, {})", c.red(), c.green(), c.blue());
    }
};

template<>
struct fmt::formatter<de_vertexwahn::Transform44f> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const de_vertexwahn::Transform44f &c, FormatContext &ctx) {
        return fmt::format_to(ctx.out(), "({})", c.matrix()(0, 0));
    }
};

template<typename ObjectType>
struct fmt::formatter<de_vertexwahn::ReferenceCounted<ObjectType>> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const de_vertexwahn::ReferenceCounted<ObjectType> &obj, FormatContext &ctx) {
        return fmt::format_to(ctx.out(), "{}", obj->to_string());
    }
};

#endif // end define De_Vertexwahn_Flatland_PopertySet_formatter_a272aef3_074e_4168_ae05_64383371db33_h
