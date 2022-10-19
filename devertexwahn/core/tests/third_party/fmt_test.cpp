/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "fmt/core.h"
#include "fmt/format.h"

#include "gmock/gmock.h"

TEST(fmt, TextFormatting) {
    std::string message = fmt::format("The answer is {}.", 42);

    EXPECT_THAT(message, "The answer is 42.");
}

struct complex {
    double a;
    double b;
};

template<>
struct fmt::formatter<complex> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(complex const &number, FormatContext &ctx) {
        return fmt::format_to(ctx.out(), "{0}+i{1}", number.a, number.b);
    }
};

TEST(fmt, CustomFormatter) {
    complex number{1, 2};
    std::string message = fmt::format("The number is {}.", number);
    EXPECT_THAT(message, "The number is 1+i2.");
}