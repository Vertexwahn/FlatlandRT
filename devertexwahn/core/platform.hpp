/*
 *  SPDX-FileCopyrightText: Copyright 2023-2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: BSD-3-Clause-LBNL
 */

// This file contains code copied from Mitsuba 3 (v3.6.2)
// The copied code is available under the following license:

/*
    Copyright (c) 2017 Wenzel Jakob <wenzel.jakob@epfl.ch>, All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    You are under no obligation whatsoever to provide any bug fixes, patches, or
    upgrades to the features, functionality or performance of the source code
    ("Enhancements") to anyone; however, if you choose to make your Enhancements
    available either publicly, or directly to the author of this software, without
    imposing a separate written license agreement for such Enhancements, then you
    hereby grant the following license: a non-exclusive, royalty-free perpetual
    license to install, use, modify, prepare derivative works, incorporate into
    other computer software, distribute, and sublicense such enhancements or
    derivative works thereof, in binary and source code form.
*/

// Modifications are licensed under the same license as the original work,
// i.e. BSD-3-Clause-LBNL

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

// Copied from https://github.com/mitsuba-renderer/mitsuba3/blob/master/include/mitsuba/core/platform.h
// Likely/unlikely macros (only on GCC/Clang)
#if defined(__GNUG__) || defined(__clang__)
#  define likely(x)   __builtin_expect(!!(x), 1)
#  define unlikely(x) __builtin_expect(!!(x), 0)
#else
#  define likely(x)       (x)
#  define unlikely(x)     (x)
#endif

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Core_Platform_4de99014_aad1_4976_9f27_4cd4afe72bdc_h
