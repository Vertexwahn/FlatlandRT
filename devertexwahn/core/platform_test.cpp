/*
 *  SPDX-FileCopyrightText: Copyright 2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/platform.hpp"

using namespace de_vertexwahn;

#include "gmock/gmock.h"

using namespace de_vertexwahn;

#if defined(DE_VERTEXWAHN_OS_LINUX)

#include <dlfcn.h>

TEST(platform, dynamic_module_find_symbol) {
    auto handle = dlopen("core/libplatform_test_library.so", RTLD_LAZY);
    ASSERT_NE(handle, nullptr);
    auto symbol = dynamic_module_find_symbol(handle, "foo");
    EXPECT_NE(symbol, nullptr);
    int (*f)(int) = (int (*)(int)) symbol;
    EXPECT_THAT(f(3), 4);
    dlclose(handle);
}

TEST(platform, dynamic_module_find_invalid_symbol) {
    auto handle = dlopen("core/libplatform_test_library.so", RTLD_LAZY);
    ASSERT_NE(handle, nullptr);
    EXPECT_DEATH(dynamic_module_find_symbol(handle, "bar"), "");
}

#endif
