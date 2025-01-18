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

#include "core/intrin.hpp"

#include <atomic>
#include <thread>

namespace de_vertexwahn {
    class spin_mutex {

    private:
#if __cplusplus < 202002L
        std::atomic_flag _flag = ATOMIC_FLAG_INIT;
#else
        std::atomic_flag _flag;// ATOMIC_FLAG_INIT is not needed as per C++20
#endif

    public:
        spin_mutex() noexcept = default;

        void lock() noexcept {
            while (_flag.test_and_set(std::memory_order::acquire)) {// acquire lock
                LUISA_INTRIN_PAUSE();
#ifdef __cpp_lib_atomic_flag_test
                while (_flag.test(std::memory_order::relaxed)) {// test lock
                    std::this_thread::yield();
                }
#endif
            }
        }
        bool try_lock() noexcept {
            return !_flag.test_and_set(std::memory_order::acquire);
        }
        void unlock() noexcept {
            _flag.clear(std::memory_order::release);// release lock
        }
    };
}// namespace de_vertexwahn
