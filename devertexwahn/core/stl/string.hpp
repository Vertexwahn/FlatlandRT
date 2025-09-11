/*
 *  SPDX-FileCopyrightText: Copyright 2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_String_67f545bb_0a93_4725_a222_af4a68fa2d4a_hpp
#define De_Vertexwahn_Core_String_67f545bb_0a93_4725_a222_af4a68fa2d4a_hpp

#include "core/namespace.hpp"

#include <string>

DE_VERTEXWAHN_BEGIN_NAMESPACE

using string = std::string;
using u16string = std::u16string_view;
using u32string = std::u32string_view;
//using u8string = std::u8string_view;
using wstring = std::wstring_view;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Core_String_67f545bb_0a93_4725_a222_af4a68fa2d4a_hpp
