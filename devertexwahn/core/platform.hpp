/*
 *  SPDX-FileCopyrightText: Copyright 2023-2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_Platform_4de99014_aad1_4976_9f27_4cd4afe72bdc_h
#define De_Vertexwahn_Core_Platform_4de99014_aad1_4976_9f27_4cd4afe72bdc_h

#include "core/namespace.hpp"

#include "boost/predef.h"
#include "boost/predef/other/endian.h"

#include <string_view>

#if BOOST_OS_WINDOWS
#define DE_VERTEXWAHN_OS_WINDOWS
#endif

#if BOOST_OS_MACOS
#define DE_VERTEXWAHN_OS_MACOS
#endif

#if BOOST_OS_LINUX
#define DE_VERTEXWAHN_OS_LINUX
#endif

#if BOOST_ENDIAN_BIG_BYTE
#define DE_VERTEXWAHN_BIG_ENDIAN 1
#else
#define DE_VERTEXWAHN_BIG_ENDIAN 0
#endif

#if BOOST_ENDIAN_LITTLE_BYTE
#define DE_VERTEXWAHN_LITTLE_ENDIAN 1
#else
#define DE_VERTEXWAHN_LITTLE_ENDIAN 0
#endif

DE_VERTEXWAHN_BEGIN_NAMESPACE

[[nodiscard]] void *dynamic_module_find_symbol(void *handle, std::string_view name) noexcept;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Core_Platform_4de99014_aad1_4976_9f27_4cd4afe72bdc_h
