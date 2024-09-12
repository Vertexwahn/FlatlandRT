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

#include "core/logging.hpp"

#include "spdlog/sinks/base_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace de_vertexwahn {

namespace detail {

static std::mutex LOGGER_MUTEX;

/*
template<typename Mt>
class SinkWithCallback : public spdlog::sinks::base_sink<Mt> {
public:
    template<class F>
    explicit SinkWithCallback(F &&_callback) noexcept
            : callback_{std::forward<F>(callback_)} {}

protected:
    void sink_it_(const spdlog::details::log_msg &msg) override {
        auto level = msg.level;
        auto level_name = spdlog::level::to_short_c_str(level);
        auto message = fmt::to_string(msg.payload);
        callback_(level_name, message.c_str());
    }
    void flush_() override {}

private:
    std::function<void(const char *, const char *)> callback_;
};
*/

static de_vertexwahn::logger LOGGER = [] {
    auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    spdlog::logger l{"console", sink};
    l.flush_on(spdlog::level::err);
#ifndef NDEBUG
    spdlog::level::level_enum log_level = spdlog::level::debug;
#else
    spdlog::level::level_enum log_level = spdlog::level::info;
#endif
    if (auto env_level_c_str = getenv("DE_VERTEXWAHN_LOG_LEVEL")) {
        std::string env_level{env_level_c_str};
        for (auto &c : env_level) { c = static_cast<char>(tolower(c)); }
        if (env_level == "verbose") {
            log_level = spdlog::level::debug;
        } else if (env_level == "info") {
            log_level = spdlog::level::info;
        } else if (env_level == "warning") {
            log_level = spdlog::level::warn;
        } else if (env_level == "error") {
            log_level = spdlog::level::err;
        } else {
            LOG_WARNING_WITH_LOCATION(
                    "Invalid log level '{}'. "
                    "Please choose from 'verbose', 'info', 'warning' and 'error'. "
                    "Fallback to default log level '{}'.",
                    env_level, std::to_string(log_level));
        }
    }
    l.set_level(log_level);
    return l;
}();

[[nodiscard]] spdlog::logger &default_logger() noexcept {
    return LOGGER;
}

void set_sink(de_vertexwahn::sink_ptr sink) noexcept {
    std::lock_guard _lock{LOGGER_MUTEX};
    LOGGER.sinks().clear();
    if (sink) {
        LOGGER.sinks().emplace_back(std::move(sink));
    }
}

/*
de_vertexwahn::sink_ptr create_sink_with_callback(std::function<void(const char *level, const char *message)> callback) noexcept {
    return std::make_shared<de_vertexwahn::detail::SinkWithCallback<std::mutex>>(std::move(callback));
}
*/

} // namespace detail

void log_level_verbose() noexcept { detail::default_logger().set_level(spdlog::level::debug); }
void log_level_info() noexcept { detail::default_logger().set_level(spdlog::level::info); }
void log_level_warning() noexcept { detail::default_logger().set_level(spdlog::level::warn); }
void log_level_error() noexcept { detail::default_logger().set_level(spdlog::level::err); }

void log_flush() noexcept { detail::default_logger().flush(); }

} // namespace de_vertexwahn
