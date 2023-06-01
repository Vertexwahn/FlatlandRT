#
#   SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
#   SPDX-License-Identifier: Apache-2.0
#

# Note: This is a first attempt to support CMake. The main build system for this project is Bazel.
# Feel free to open a pull request for improvements of the CMake build support.

add_library(flatland.core SHARED
    logging.h
    namespace.h
    non_copyable.h
    object.cpp
    object.h
    object_factory.h
    object_factory.cpp
    property_set.cpp
    property_set.h
    reference_counted.h
    timer.cpp
    timer.h
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
