/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_NonCopyable_3f4e4740_1b8d_4586_bd61_5a05427206e4_h
#define Flatland_NonCopyable_3f4e4740_1b8d_4586_bd61_5a05427206e4_h

#include "flatland/core/namespace.h"

FLATLAND_BEGIN_NAMESPACE

struct NonCopyable {
	NonCopyable& operator=(const NonCopyable&) = delete;
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable() = default;
};

FLATLAND_END_NAMESPACE

#endif // end define Flatland_NonCopyable_3f4e4740_1b8d_4586_bd61_5a05427206e4_h
