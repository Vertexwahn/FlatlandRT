/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_ReferenceCounted_238dd615_2085_4070_b244_14af1a114dc8_h
#define De_Vertexwahn_Core_ReferenceCounted_238dd615_2085_4070_b244_14af1a114dc8_h

#include "core/namespace.hpp"

#include <memory>

DE_VERTEXWAHN_BEGIN_NAMESPACE

template <typename ObjectType>
using ReferenceCounted = std::shared_ptr<ObjectType>;

template <typename ObjectType, typename... Args>
ReferenceCounted<ObjectType> make_reference_counted(Args&&... args) {
    return std::make_shared<ObjectType>(std::forward<Args>(args)...);
}

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Core_ReferenceCounted_238dd615_2085_4070_b244_14af1a114dc8_h
