/*
 *  SPDX-FileCopyrightText: Copyright 2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "boost/predef.h"

#include "fmt/core.h"
#include "fmt/format.h"

#include <iostream>
#include <map>
#include <sstream>
#include <string>

std::string semantic_versioning_string(int major, int minor, int patch) {
    return fmt::format("{}.{}.{}", major, minor, patch);
}

// See https://dev.to/yumetodo/list-of-mscver-and-mscfullver-8nd
std::string vs_product_name(int version) {
    std::map<int, std::string> versions {
        {1936, "Visual Studio 2022 17.6.2"},
        {1937, "Visual Studio 2022 17.7.0"},
        {1938, "Visual Studio 2022 17.8.3"},
        {1939, "Visual Studio 2022 17.9.2"},
    };

    auto it = versions.find(version);

    if(it != std::end(versions)) {
        return it->second;
    } 
    else {
        return fmt::format("Unknown Visual Studio version {}", version);
    }
}

int main() {
#if __apple_build_version__
    std::cout << "Apple clang version " << semantic_versioning_string(__clang_major__, __clang_minor__, __clang_patchlevel__);
#elif BOOST_COMP_CLANG
    std::cout << "Clang " << semantic_versioning_string(__clang_major__, __clang_minor__, __clang_patchlevel__);
#elif BOOST_COMP_GNUC
    std::cout << "GCC " << semantic_versioning_string(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#elif BOOST_COMP_MSVC
    std::cout << vs_product_name(_MSC_VER);
#else
    std::cout << "Unknown compiler";
#endif

    std::cout << std::endl;

    return 0;
}
