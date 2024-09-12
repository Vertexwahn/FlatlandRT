// The following code is from
//   [LuisaCompute](https://github.com/LuisaGroup/LuisaCompute/blob/d2f0c6536bc844bde00d732b0fe1ea5aedfb976f/src/backends/cuda/optix_api.h) by LuisaGroup
// The following code is from
//   [Dr.JIT-Core](https://github.com/mitsuba-renderer/drjit-core) by Wenzel Jakob
//
// Originally licensed under BSD-3-Clause:
//
// Copyright (c) 2021 Wenzel Jakob <wenzel.jakob@epfl.ch>, All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "core/platform.hpp"
#include "core/timer.hpp"
#include "core/logging.hpp"

#if defined(DE_VERTEXWAHN_OS_WINDOWS) // Windows related code

#ifndef UNICODE
#define UNICODE 1
#endif

#ifndef NOMINMAX
#define NOMINMAX 1
#endif

#include <windows.h>

#include "fmt/core.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

namespace detail {

[[nodiscard]] std::string win32_last_error_message() {
    // Retrieve the system error message for the last-error code
    void *buffer = nullptr;
    auto err_code = GetLastError();
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        err_code,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&buffer,
        0, nullptr);
    std::string err_msg{fmt::format("{} (code = 0x{:x}).", static_cast<char *>(buffer), err_code)};
    LocalFree(buffer);
    return err_msg;
}

}// namespace detail

[[nodiscard]] void *dynamic_module_find_symbol(void *handle, std::string_view name_view) noexcept {
    static thread_local std::string name;
    name = name_view;
    auto symbol = GetProcAddress(reinterpret_cast<HMODULE>(handle), name.c_str());
    if (symbol == nullptr) [[unlikely]] {
        LOG_ERROR_WITH_LOCATION("Failed to load symbol '{}', reason: {}.",
                                  name, detail::win32_last_error_message());
    }
    return reinterpret_cast<void *>(symbol);
}

DE_VERTEXWAHN_END_NAMESPACE

#elif defined(DE_VERTEXWAHN_OS_LINUX) // Linux related code

//#include <unistd.h>
#include <dlfcn.h>
//#include <execinfo.h>
#include <cxxabi.h>

DE_VERTEXWAHN_BEGIN_NAMESPACE

void *dynamic_module_find_symbol(void *handle, std::string_view name_view) noexcept {
    static thread_local std::string name;
    name = name_view;
    Timer timer;
    auto symbol = dlsym(handle, name.c_str());
    if (symbol == nullptr) [[unlikely]] {
        LOG_ERROR_WITH_LOCATION("Failed to load symbol '{}', reason: {}.",
                                  name, dlerror());
    }
    LOG_VERBOSE_WITH_LOCATION(
            "Loading dynamic symbol '{}' in {} ms.",
            name, timer.elapsed_milliseconds());
    return symbol;
}

DE_VERTEXWAHN_END_NAMESPACE

#endif
