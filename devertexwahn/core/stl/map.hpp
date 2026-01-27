/*
 *  SPDX-FileCopyrightText: Copyright 2026 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_map_d6f72a83_1a73_4521_a5d1_df2f7b916668_hpp
#define De_Vertexwahn_Core_map_d6f72a83_1a73_4521_a5d1_df2f7b916668_hpp

#include "core/namespace.hpp"

#include <map>

DE_VERTEXWAHN_BEGIN_NAMESPACE

template<
    class Key,
    class T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<std::pair<const Key, T>>
>
using map = std::map<Key, T, Compare, Allocator>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Core_map_d6f72a83_1a73_4521_a5d1_df2f7b916668_hppv
