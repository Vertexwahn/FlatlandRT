/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_Object_ccb5c1f6_846a_4ff1_82b1_06a8edae8bf0_h
#define De_Vertexwahn_Core_Object_ccb5c1f6_846a_4ff1_82b1_06a8edae8bf0_h

#include "core/namespace.hpp"
#include "core/non_copyable.hpp"

#include "core/stl/string.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

class Object : NonCopyable {
public:
    Object() = default;

    virtual ~Object() = default;

    [[nodiscard]]
    virtual string to_string() const;
};

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Core_Object_ccb5c1f6_846a_4ff1_82b1_06a8edae8bf0_h
