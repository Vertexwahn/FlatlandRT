"""
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
"""

def cc_test_flatland(name, srcs, additional_deps = [], **kwargs):
    native.cc_test(
        name = name,
        timeout = "short",
        srcs = srcs,
        deps = [
            "//srcs:flatland",
            "@hypothesis//:hypothesis",
            "@googletest//:gtest_main",
        ],
        **kwargs
    )
