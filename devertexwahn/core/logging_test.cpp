/*
 *  SPDX-FileCopyrightText: Copyright 2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/tests/test_logger.hpp"

#include "core/logging.hpp"
#include "core/platform.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <fstream>

using namespace de_vertexwahn;

TEST(Logging, HelloWorldVerbose) {
    // When using ::testing::internal::CaptureStdout and
    // ::testing::internal::GetCapturedStdout the following test are failing
    // on Windows in fastbuild and opt build mode. dbg mode works.
    // Therefore, we use a custom TestLogger class to capture the output.
    // Anyway, those functions are internal and should not be used.
    //::testing::internal::CaptureStdout();
    TestLogger test_logger;
    log_level_verbose();
    const std::string log_message{"Hello World! (verbose)"};
    LOG_VERBOSE("{}", log_message);
    log_flush();
    std::string output = test_logger.content(); // testing::internal::GetCapturedStdout();
    EXPECT_THAT(output, ::testing::HasSubstr(log_message));
}

TEST(Logging, HelloWorldInfo) {
    TestLogger test_logger;
    log_level_info();
    const std::string log_message{"Hello World! (info)"};
    LOG_INFO("{}", log_message);
    log_flush();
    std::string output = test_logger.content();
    EXPECT_THAT(output, ::testing::HasSubstr(log_message));
}

TEST(Logging, HelloWorldWarning) {
    TestLogger test_logger;
    log_level_warning();
    const std::string log_message{"Hello World! (warning)"};
    LOG_WARNING("{}", log_message);
    log_flush();
    std::string output = test_logger.content();
    EXPECT_THAT(output, ::testing::HasSubstr(log_message));
}

TEST(Logging, HelloWorldError) {
    TestLogger test_logger;
    log_level_error();
    const std::string log_message{"Hello World! (error)"};

    EXPECT_DEATH(LOG_ERROR("{}", log_message), "");
    //EXPECT_EXIT(LOG_ERROR("{}", log_message), ::testing::KilledBySignal(SIGABRT), ".*");
  
    log_flush();
    std::string output = test_logger.content();
    EXPECT_THAT(output, ::testing::HasSubstr(log_message));
}

TEST(Logging, LogLevelInfo) {
    TestLogger test_logger;

    const std::string log_message_verbose{"Hello World! (verbose)"};
    const std::string log_message_info{"Hello World! (info)"};

    log_level_info(); // do not show verbose messages
    LOG_VERBOSE("{}", log_message_verbose);
    LOG_INFO("{}", log_message_info);

    log_flush();

    std::string output = test_logger.content();
    EXPECT_THAT(output, ::testing::Not(::testing::HasSubstr(log_message_verbose)));
    EXPECT_THAT(output, ::testing::HasSubstr(log_message_info));
}

TEST(Logging, LogLevelWarning) {
    TestLogger test_logger;

    const std::string log_message_verbose{"Hello World! (verbose)"};
    const std::string log_message_info{"Hello World! (info)"};
    const std::string log_message_warning{"Hello World! (warning)"};

    log_level_warning(); // do not show verbose and info messages
    LOG_VERBOSE("{}", log_message_verbose);
    LOG_INFO("{}", log_message_info);
    LOG_WARNING("{}", log_message_warning);

    log_flush();

    std::string output = test_logger.content();
    EXPECT_THAT(output, ::testing::Not(::testing::HasSubstr(log_message_verbose)));
    EXPECT_THAT(output, ::testing::Not(::testing::HasSubstr(log_message_info)));
    EXPECT_THAT(output, ::testing::HasSubstr(log_message_warning));
}

/*
TEST(Logging, sink) {
    int call_times = 0;
    std::function<void(const char *level, const char *message)> foo = [&call_times](const char *level, const char *message) {
        call_times++;
    };

    //auto sink = ::de_vertexwahn::detail::create_sink_with_callback(foo);
    auto sink = ::de_vertexwahn::detail::create_sink_with_callback(std::bind(&Bar::log, &bar, std::placeholders::_1, std::placeholders::_2));
    ::de_vertexwahn::detail::set_sink(sink);

    log_level_verbose();
    LOG_VERBOSE("Hello!");
    log_flush();

    //EXPECT_THAT(call_times, 1);
}
*/
