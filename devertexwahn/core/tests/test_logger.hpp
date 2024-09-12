/*
 *  SPDX-FileCopyrightText: Copyright 2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>
#include "core/logging.hpp"

namespace de_vertexwahn {

class TestLogger {
public:
    TestLogger();

    std::string content() const;

private:
    inline static std::string LOG_FILENAME{"logs/test_logger.txt"};
};

}
