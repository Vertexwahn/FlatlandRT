#
#   SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
#   SPDX-License-Identifier: Apache-2.0
#

# Note: This is a first attempt to support CMake. The main build system for this project is Bazel.
# Feel free to open a pull request for improvements of the CMake build support.

add_library(flatland.core SHARED
        logging.hpp
        namespace.hpp
        non_copyable.hpp
        object.cpp
        object.hpp
        object_factory.hpp
        object_factory.cpp
        property_set.cpp
        property_set.hpp
        reference_counted.hpp
        timer.cpp
        timer.hpp
)

target_include_directories(flatland.core
    PUBLIC
        ..
)

target_link_libraries(flatland.core 
  PRIVATE 
    #gtest_main 
    fmt::fmt 
)
