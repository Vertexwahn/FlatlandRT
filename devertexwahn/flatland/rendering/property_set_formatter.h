/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_PopertySet_formatter_a272aef3_074e_4168_ae05_64383371db33_h
#define Flatland_PopertySet_formatter_a272aef3_074e_4168_ae05_64383371db33_h

#include "flatland/math/transform.h"
#include "flatland/math/util.h"
#include "flatland/core/object.h"
#include "flatland/core/reference_counted.h"
#include "flatland/imaging/color.h"

#include "fmt/format.h"

template<>
struct fmt::formatter<flatland::Vector2f> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const flatland::Vector2f &v, FormatContext &ctx) {
        return format_to(ctx.out(), "({}, {})", v.x(), v.y());
    }
};

template<>
struct fmt::formatter<flatland::Vector2d> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const flatland::Vector2d &v, FormatContext &ctx) {
        return format_to(ctx.out(), "({}, {})", v.x(), v.y());
    }
};

template<>
struct fmt::formatter<flatland::Vector3f> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const flatland::Vector3f &v, FormatContext &ctx) {
        return format_to(ctx.out(), "({}, {}, {})", v.x(), v.y(), v.z());
    }
};

template<>
struct fmt::formatter<flatland::Vector3d> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const flatland::Vector3f &v, FormatContext &ctx) {
        return format_to(ctx.out(), "({}, {}, {})", v.x(), v.y(), v.z());
    }
};

template<>
struct fmt::formatter<flatland::Point2f> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const flatland::Point2f &v, FormatContext &ctx) {
        return format_to(ctx.out(), "({}, {})", v.x(), v.y());
    }
};

template<>
struct fmt::formatter<flatland::Point2d> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const flatland::Point2d &v, FormatContext &ctx) {
        return format_to(ctx.out(), "({}, {})", v.x(), v.y());
    }
};

template<>
struct fmt::formatter<flatland::Color3f> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const flatland::Color3f &c, FormatContext &ctx) {
        return format_to(ctx.out(), "({}, {}, {})", c.red(), c.green(), c.blue());
    }
};

template<>
struct fmt::formatter<flatland::Transform44f> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const flatland::Transform44f &c, FormatContext &ctx) {
        return format_to(ctx.out(), "({})", c.matrix()(0, 0));
    }
};

template<typename ObjectType>
struct fmt::formatter<flatland::ReferenceCounted<ObjectType>> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const flatland::ReferenceCounted<ObjectType> &obj, FormatContext &ctx) {
        return format_to(ctx.out(), "{}", obj->to_string());
    }
};

#endif // end define Flatland_PopertySet_formatter_a272aef3_074e_4168_ae05_64383371db33_h