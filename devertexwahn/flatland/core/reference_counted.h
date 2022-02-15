/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Core_ReferenceCounted_238dd615_2085_4070_b244_14af1a114dc8_h
#define Flatland_Core_ReferenceCounted_238dd615_2085_4070_b244_14af1a114dc8_h

#include "flatland/core/namespace.h"

#include <memory>

FLATLAND_BEGIN_NAMESPACE

template <typename ObjectType>
using ReferenceCounted = std::shared_ptr<ObjectType>;

template <typename ObjectType, typename... Args>
ReferenceCounted<ObjectType> make_reference_counted(Args&&... args) {
    return std::make_shared<ObjectType>(std::forward<Args>(args)...);
}

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Core_ReferenceCounted_238dd615_2085_4070_b244_14af1a114dc8_h
