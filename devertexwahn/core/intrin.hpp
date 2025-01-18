// The following code has been copied from https://github.com/LuisaGroup/LuisaCompute
// and is licensed under the following license:
/*
    BSD 3-Clause License

    Copyright (c) 2023, LuisaGroup
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// Modifications are licensed under the same license as the original work, i.e. BSD 3-Clause License

#pragma once

#if defined(__x86_64__) || defined(_M_X64)
#define LUISA_ARCH_X86_64
#elif defined(__aarch64__)
#define LUISA_ARCH_ARM64
#else
#error Unsupported architecture
#endif

#if defined(LUISA_ARCH_X86_64)

#include <immintrin.h>
#include <cstdint>
#include <cassert>

#define LUISA_INTRIN_PAUSE() _mm_pause()

namespace luisa {
    using float16_t = int16_t;
    using float32x4_t = __m128;
}// namespace luisa

#elif defined(LUISA_ARCH_ARM64)

#include <arm_neon.h>

namespace luisa {
using float16_t = ::float16_t;
using float32x4_t = ::float32x4_t;
}// namespace luisa

#define LUISA_INTRIN_PAUSE() asm volatile("isb")

#else

#include <thread>
#define LUISA_INTRIN_PAUSE() std::this_thread::yield()

#endif

////////////// assume
#ifdef NDEBUG // assume only enabled in non-debug mode.
#if defined(__clang__)// Clang
#define LUISA_ASSUME(x) (__builtin_assume(x))
#elif defined(_MSC_VER)// MSVC
#define LUISA_ASSUME(x) (__assume(x))
#else// GCC
#define LUISA_ASSUME(x) \
    if (!(x)) __builtin_unreachable()
#endif
#else
#define LUISA_ASSUME(expression) assert(expression)
#endif
