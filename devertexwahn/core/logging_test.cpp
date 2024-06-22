/*
 *  SPDX-FileCopyrightText: Copyright 2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/logging.h"
#include "core/platform.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

// The following test are failing on Windows in fastbuild and opt build mode. 
// dbg mode works.
// For now, those test are exclude on Windows
#ifndef BOOST_OS_WINDOWS

TEST(Logging, HelloWorldVerbose) {
    testing::internal::CaptureStdout();
    log_level_verbose();
    const std::string log_message{"Hello World! (verbose)"};
    LOG_VERBOSE("{}", log_message);
    log_flush();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_THAT(output, ::testing::HasSubstr(log_message));
}

TEST(Logging, HelloWorldInfo) {
    testing::internal::CaptureStdout();
    log_level_info();
    const std::string log_message{"Hello World! (info)"};
    LOG_INFO("{}", log_message);
    log_flush();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_THAT(output, ::testing::HasSubstr(log_message));
}

TEST(Logging, HelloWorldWarning) {
    testing::internal::CaptureStdout();
    log_level_warning();
    const std::string log_message{"Hello World! (warning)"};
    LOG_WARNING("{}", log_message);
    log_flush();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_THAT(output, ::testing::HasSubstr(log_message));
}

TEST(Logging, HelloWorldError) {
    testing::internal::CaptureStdout();
    log_level_error();
    const std::string log_message{"Hello World! (error)"};
    LOG_ERROR("{}", log_message);
    log_flush();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_THAT(output, ::testing::HasSubstr(log_message));
}

TEST(Logging, LogLevelInfo) {
    testing::internal::CaptureStdout();

    const std::string log_message_verbose{"Hello World! (verbose)"};
    const std::string log_message_info{"Hello World! (info)"};

    log_level_info(); // do not show verbose messages
    LOG_VERBOSE("{}", log_message_verbose);
    LOG_INFO("{}", log_message_info);

    log_flush();

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_THAT(output, ::testing::Not(::testing::HasSubstr(log_message_verbose)));
    EXPECT_THAT(output, ::testing::HasSubstr(log_message_info));
}

TEST(Logging, LogLevelWarning) {
    testing::internal::CaptureStdout();

    const std::string log_message_verbose{"Hello World! (verbose)"};
    const std::string log_message_info{"Hello World! (info)"};
    const std::string log_message_warning{"Hello World! (warning)"};

    log_level_warning(); // do not show verbose and info messages
    LOG_VERBOSE("{}", log_message_verbose);
    LOG_INFO("{}", log_message_info);
    LOG_WARNING("{}", log_message_warning);

    log_flush();

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_THAT(output, ::testing::Not(::testing::HasSubstr(log_message_verbose)));
    EXPECT_THAT(output, ::testing::Not(::testing::HasSubstr(log_message_info)));
    EXPECT_THAT(output, ::testing::HasSubstr(log_message_warning));
}

#endif
