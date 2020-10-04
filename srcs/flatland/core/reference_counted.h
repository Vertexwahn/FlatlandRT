/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_ReferenceCounted_238dd615_2085_4070_b244_14af1a114dc8_h
#define Flatland_ReferenceCounted_238dd615_2085_4070_b244_14af1a114dc8_h

#include "flatland/core/namespace.h"

#include <memory>

FLATLAND_BEGIN_NAMESPACE

template <typename T> using ReferenceCounted = std::shared_ptr<T>;

template <typename T, typename... Args>
ReferenceCounted<T> makeReferenceCounted(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

FLATLAND_END_NAMESPACE

#endif // end define Flatland_ReferenceCounted_238dd615_2085_4070_b244_14af1a114dc8_h
