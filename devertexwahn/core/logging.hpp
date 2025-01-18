/*
 *  The logging functionality that can be found here was copied from LuisaCompute (sse https://github.com/LuisaGroup/LuisaCompute)
 *  and modified by Julian Amann - the code (including the modifications) are available under BSD 3-Clause License.
 * 
 *  The next section contains the original copyright information:
 */

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

#pragma once
#ifndef De_Vertexwahn_Core_Logging_6d12aed9_2efe_4003_9708_91e52ca2e8b7_hpp
#define De_Vertexwahn_Core_Logging_6d12aed9_2efe_4003_9708_91e52ca2e8b7_hpp

#include "spdlog/spdlog.h"

namespace de_vertexwahn {

using spdlog::logger;
using spdlog::sink_ptr;

namespace detail {
    [[nodiscard]] de_vertexwahn::logger &default_logger() noexcept;
    void set_sink(de_vertexwahn::sink_ptr sink) noexcept;
    de_vertexwahn::sink_ptr create_sink_with_callback(
            std::function<void(const char *level,
                               const char *message)>
    callback) noexcept;
} // namespace de_vertexwahn::detail

template<typename... Args>
inline void log_verbose(Args &&...args) noexcept {
    detail::default_logger().debug(std::forward<Args>(args)...);
}

template<typename... Args>
inline void log_info(Args &&...args) noexcept {
    detail::default_logger().info(std::forward<Args>(args)...);
}

template<typename... Args>
inline void log_warning(Args &&...args) noexcept {
    detail::default_logger().warn(std::forward<Args>(args)...);
}

template<typename... Args>
[[noreturn]] inline void log_error(Args &&...args) noexcept {
    detail::default_logger().error(std::forward<Args>(args)...);
    std::abort();
}

/// Set log level as verbose
void log_level_verbose() noexcept;
/// Set log level as info
void log_level_info() noexcept;
/// Set log level as warning
void log_level_warning() noexcept;
/// Set log level as error
void log_level_error() noexcept;

/// flush the logs
void log_flush() noexcept;

} // namespace de_vertexwahn

/**
 * @brief Verbose logging
 *
 * Ex. LOG_VERBOSE("function {} returns {}", functionName, functionReturnInt);
 */
#define LOG_VERBOSE(fmt_string, ...) \
    ::de_vertexwahn::log_verbose(::fmt::format(fmt_string  __VA_OPT__(, ) __VA_ARGS__))

/**
 * @brief Info logging
 *
 * Ex. LOG_INFO("function {} returns {}", functionName, functionReturnInt);
 */
#define LOG_INFO(fmt_string, ...) \
    ::de_vertexwahn::log_info(::fmt::format(fmt_string  __VA_OPT__(, ) __VA_ARGS__))
/**
 * @brief Warning logging
 *
 * Ex. LOG_WARNING("function {} returns {}", functionName, functionReturnInt);
 */
#define LOG_WARNING(fmt_string, ...) \
    ::de_vertexwahn::log_warning(::fmt::format(fmt_string  __VA_OPT__(, ) __VA_ARGS__))
/**
 * @brief Error logging
 *
 * After logging error message, the program will be aborted.
 * Ex. LOG_ERROR("function {} returns {}", functionName, functionReturnInt);
 */
#define LOG_ERROR(fmt_string, ...) \
    ::de_vertexwahn::log_error(::fmt::format(fmt_string  __VA_OPT__(, ) __VA_ARGS__))

/// LOG_VERBOSE with file and line information
#define LOG_VERBOSE_WITH_LOCATION(fmt_string, ...) \
    ::de_vertexwahn::log_verbose(::fmt::format(fmt_string " [{}:{}]" __VA_OPT__(, ) __VA_ARGS__, __FILE__, __LINE__))
/// LOG_INFO with file and line information
#define LOG_INFO_WITH_LOCATION(fmt_string, ...) \
    ::de_vertexwahn::log_info(::fmt::format(fmt_string " [{}:{}]" __VA_OPT__(, ) __VA_ARGS__, __FILE__, __LINE__))
/// LOG_WARNING with file and line information
#define LOG_WARNING_WITH_LOCATION(fmt_string, ...) \
    ::de_vertexwahn::log_warning(::fmt::format(fmt_string " [{}:{}]" __VA_OPT__(, ) __VA_ARGS__, __FILE__, __LINE__))
/// LOG_ERROR with file and line information
#define LOG_ERROR_WITH_LOCATION(fmt_string, ...) \
    ::de_vertexwahn::log_error(::fmt::format(fmt_string " [{}:{}]" __VA_OPT__(, ) __VA_ARGS__, __FILE__, __LINE__))

#define LOG_NOT_IMPLEMENTED() \
    LOG_ERROR_WITH_LOCATION("Not implemented.")

#define LOG_ASSERT(x, fmt_string, ...)                  \
    do {                                                \
        if (!(x)) [[unlikely]] {                        \
            auto msg = fmt::format(                     \
                fmt_string __VA_OPT__(, ) __VA_ARGS__); \
            LOG_ERROR_WITH_LOCATION(                    \
                "Assertion '{}' failed: {}",            \
                #x, msg);                               \
        }                                               \
    } while (false)


#endif // end define De_Vertexwahn_Core_Logging_6d12aed9_2efe_4003_9708_91e52ca2e8b7_hpp
