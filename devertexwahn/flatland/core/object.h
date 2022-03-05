/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Core_Object_ccb5c1f6_846a_4ff1_82b1_06a8edae8bf0_h
#define Flatland_Core_Object_ccb5c1f6_846a_4ff1_82b1_06a8edae8bf0_h

#include "flatland/core/namespace.h"
#include "flatland/core/non_copyable.h"

#include <string>

FLATLAND_BEGIN_NAMESPACE

class Object : NonCopyable {
public:
    Object() = default;

    virtual ~Object() = default;

    [[nodiscard]]
    virtual std::string to_string() const;
};

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Core_Object_ccb5c1f6_846a_4ff1_82b1_06a8edae8bf0_h
