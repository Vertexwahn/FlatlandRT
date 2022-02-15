"""
    SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
    SPDX-License-Identifier: Apache-2.0
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
