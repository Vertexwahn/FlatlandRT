"""
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
"""

def flatland_linker_opts():
    native.config_setting(
        name = "compiler_gcc_7_5_0",
        values = {
            "define": "compiler=gcc7_5_0",
        },
    )

FLATLAND_DEFAULT_LINKOPTS = select({
    "compiler_gcc_7_5_0": ["-lstdc++fs"],  # needed by GCC 7.5.0 to support std::filesystem
    "//conditions:default": [""],  # LLVM, MSVC
})
