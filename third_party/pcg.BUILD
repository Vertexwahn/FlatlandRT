"""
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
"""

cc_library(
    name = "pcg",
    srcs = [
        "include/pcg_extras.hpp",
        "include/pcg_random.hpp",
        "include/pcg_uint128.hpp",
    ],
    hdrs = ["include/pcg_random.hpp"],
    includes = ["include/"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
)
