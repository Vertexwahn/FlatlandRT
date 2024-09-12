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

TEST(Logging, EnvironmentVariableSetToWarning) {
    TestLogger test_logger;

    const std::string log_message_verbose{"Hello World! (verbose)"};
    const std::string log_message_info{"Hello World! (info)"};
    const std::string log_message_warning{"Hello World! (warning)"};

    LOG_VERBOSE("{}", log_message_verbose);
    LOG_INFO("{}", log_message_info);
    LOG_WARNING("{}", log_message_warning);

    log_flush();

    std::string output = test_logger.content();

    EXPECT_THAT(output, ::testing::Not(::testing::HasSubstr(log_message_verbose)));

    if(std::string(getenv("DE_VERTEXWAHN_LOG_LEVEL"))=="info") {
        EXPECT_THAT(output, ::testing::HasSubstr(log_message_info));
    } else {
        EXPECT_THAT(output, ::testing::Not(::testing::HasSubstr(log_message_info)));
    }

    EXPECT_THAT(output, ::testing::HasSubstr(log_message_warning));
}
