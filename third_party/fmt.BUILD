"""
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
"""

cc_library(
    name = "fmt",
    srcs = [
        "include/fmt/chrono.h",
        "include/fmt/color.h",
        "include/fmt/compile.h",
        "include/fmt/core.h",
        "include/fmt/format.h",
        "include/fmt/format-inl.h",
        "include/fmt/locale.h",
        "include/fmt/os.h",
        "include/fmt/ostream.h",
        "include/fmt/posix.h",
        "include/fmt/printf.h",
        "include/fmt/ranges.h",
        "src/format.cc",
        "src/os.cc",
    ],
    hdrs = [
        "include/fmt/core.h",
        "include/fmt/format.h",
    ],
    includes = ["include/"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
)
