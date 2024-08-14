/*
 *  SPDX-FileCopyrightText: Copyright 2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "boost/predef.h"

#include "fmt/core.h"
#include "fmt/format.h"

//#include <iostream>
#include <map>
#include <string>

std::string semantic_versioning_string(int major, int minor, int patch) {
    return fmt::format("{}.{}.{}", major, minor, patch);
}

// See https://dev.to/yumetodo/list-of-mscver-and-mscfullver-8nd
std::string vs_product_name(int version) {
    std::map<int, std::string> versions {
        {1929, "Visual Studio 2019 16.11.2"},
        {1933, "Visual Studio 2022 17.3.6"},
        {1936, "Visual Studio 2022 17.6.2"},
        {1937, "Visual Studio 2022 17.7.0"},
        {1938, "Visual Studio 2022 17.8.3"},
        {1939, "Visual Studio 2022 17.9.2"},
        {1940, "Visual Studio 2022 17.10.1"},
    };

    auto it = versions.find(version);

    if(it != std::end(versions)) {
        return it->second;
    } 
    else {
        return fmt::format("Unknown Visual Studio version {}", version);
    }
}

void print_compiler_version() {
#if __apple_build_version__
    auto version = semantic_versioning_string(__clang_major__, __clang_minor__, __clang_patchlevel__);
    fmt::println("Apple clang version {}", version);
#elif BOOST_COMP_CLANG
    auto version = semantic_versioning_string(__clang_major__, __clang_minor__, __clang_patchlevel__);
    fmt::println("GCC {}", version);
#elif BOOST_COMP_GNUC
    auto version =  semantic_versioning_string(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    fmt::print("GCC {}", version);
#elif BOOST_COMP_MSVC
    fmt::println("{}", vs_product_name(_MSC_VER));
#else
    fmt::print("Unknown compiler");
#endif
}

std::string to_std_version(long magic_number) {
    std::string cpp11 = "C++11";
    std::string cpp17 = "C++17";
    std::string cpp_unknown = "C++Unknown";

    switch(magic_number) {
        case 200809L:
            return cpp11;
        case 201411L:
            return cpp17;
        default:
            return cpp_unknown;
    }
}

void feature_testing() {
    // See also: https://en.cppreference.com/w/cpp/feature_test

#ifdef __cpp_attributes
    fmt::println("has __cpp_attributes ({})", to_std_version(__cpp_attributes));
#else
    fmt::println("no __cpp_attributes");
#endif

#ifdef __cpp_conditional_explicit
    fmt::println("has __cpp_conditional_explicit");
#else
    fmt::println("no __cpp_conditional_explicit");
#endif

#ifdef __cpp_range_based_for
    fmt::println("has __cpp_range_based_for");
#else
    fmt::println("no __cpp_range_based_for");
#endif

#ifdef __cpp_static_assert
    fmt::println("has __cpp_static_assert");
#else
    fmt::println("no __cpp_static_assert");
#endif
}

int main() {
    print_compiler_version();

    feature_testing();

    return 0;
}
