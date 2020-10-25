/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Object_ccb5c1f6_846a_4ff1_82b1_06a8edae8bf0_h
#define Flatland_Object_ccb5c1f6_846a_4ff1_82b1_06a8edae8bf0_h

#include "flatland/core/namespace.h"
#include "flatland/core/non_copyable.h"

FLATLAND_BEGIN_NAMESPACE

class Object : NonCopyable {
public:
    Object() {
    }
    virtual ~Object() {
    }
};

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Object_ccb5c1f6_846a_4ff1_82b1_06a8edae8bf0_h
