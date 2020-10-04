/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

/*
    The reason why this file exist is that gcc 7.5.0 does support std::filesystem only via <experimental/filesystem>.
    Support of gcc 7.5.0 will be dropped soon.
    Then we can get rid of this file.
*/

#pragma once
#ifndef Flatland_memory_e589eb94_cb15_4ce5_8649_80223e73d7f1_h
#define Flatland_memory_e589eb94_cb15_4ce5_8649_80223e73d7f1_h

#include "flatland/core/namespace.h"

#if _MSC_VER
#include <filesystem>
#elif __llvm__
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

FLATLAND_BEGIN_NAMESPACE

#if _MSC_VER
namespace fs = std::filesystem;
#elif __llvm__
namespace fs = std::filesystem;
#else
namespace fs = std::experimental::filesystem;
#endif

FLATLAND_END_NAMESPACE

#endif // end define Flatland_memory_e589eb94_cb15_4ce5_8649_80223e73d7f1_h
