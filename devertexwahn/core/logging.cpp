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

// There a different options for logging
// Currently we use glog - but abseil logging seems to be the better alternative
// Also spdlog is very famous

// Logging in LuisaCompute:
// https://github.com/LuisaGroup/LuisaCompute/blob/stable/src/core/logging.cpp
// https://github.com/LuisaGroup/LuisaCompute/blob/stable/include/luisa/core/logging.h

// Logging in Mitsuba 3
// https://github.com/mitsuba-renderer/mitsuba3/blob/master/include/mitsuba/core/logger.h

#include "core/logging.h"

#include "spdlog/sinks/base_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace de_vertexwahn {

namespace detail {

static de_vertexwahn::logger LOGGER = [] {
    auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    spdlog::logger l{"console", sink};
    l.flush_on(spdlog::level::err);
#ifndef NDEBUG
    spdlog::level::level_enum log_level = spdlog::level::debug;
#else
    spdlog::level::level_enum log_level = spdlog::level::info;
#endif

    l.set_level(log_level);
    return l;
}();

[[nodiscard]] spdlog::logger &default_logger() noexcept {
    return LOGGER;
}

}

void log_level_verbose() noexcept { detail::default_logger().set_level(spdlog::level::debug); }
void log_level_info() noexcept { detail::default_logger().set_level(spdlog::level::info); }
void log_level_warning() noexcept { detail::default_logger().set_level(spdlog::level::warn); }
void log_level_error() noexcept { detail::default_logger().set_level(spdlog::level::err); }

void log_flush() noexcept { detail::default_logger().flush(); }

} // namespace de_vertexwahn