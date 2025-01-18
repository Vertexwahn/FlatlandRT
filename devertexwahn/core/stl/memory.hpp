/*
 *  SPDX-FileCopyrightText: Copyright 2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_Memory_baf410bb_d4ca_432d_8d74_e970f99c8004_hpp
#define De_Vertexwahn_Core_Memory_baf410bb_d4ca_432d_8d74_e970f99c8004_hpp

#include "core/namespace.hpp"

#include <memory>

DE_VERTEXWAHN_BEGIN_NAMESPACE

inline namespace size_literals {

[[nodiscard]] constexpr auto operator""_k(unsigned long long size) noexcept {
    return static_cast<size_t>(size * 1024u);
}

[[nodiscard]] constexpr auto operator""_M(unsigned long long size) noexcept {
    return static_cast<size_t>(size * 1024u * 1024u);
}

[[nodiscard]] constexpr auto operator""_G(unsigned long long size) noexcept {
    return static_cast<size_t>(size * 1024u * 1024u * 1024u);
}

}// namespace size_literals

using std::const_pointer_cast;
using std::dynamic_pointer_cast;
using std::make_shared;
using std::make_unique;
using std::reinterpret_pointer_cast;
using std::shared_ptr;
using std::static_pointer_cast;
using std::unique_ptr;
using std::weak_ptr;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Core_Memory_baf410bb_d4ca_432d_8d74_e970f99c8004_hpp
