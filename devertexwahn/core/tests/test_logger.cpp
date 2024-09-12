/*
 *  SPDX-FileCopyrightText: Copyright 2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/tests/test_logger.hpp"

#include "spdlog/sinks/basic_file_sink.h"

#include <fstream>
#include <iostream>

namespace de_vertexwahn {
namespace detail {
    std::string read_file(std::string_view filename) {
        std::ifstream file(filename.data());
        std::string line;
        std::string content;
        while (std::getline(file, line)) {
            content += line;
        }
        return content;
    }
}

TestLogger::TestLogger() {
    // check if log file exists and delete it
    std::ifstream file(LOG_FILENAME);
    if (file.good()) {
        file.close();
        std::remove(LOG_FILENAME.c_str());
    }

    // let the log level unmodified
    //auto current_log_level = de_vertexwahn::detail::default_logger().level();

    // create a file sink
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(LOG_FILENAME, true);

    /*
    auto fl = file_sink->level();

    if(fl != current_log_level) {
        std::cout << "muhhhhhhhhhhhhh" << std::endl;
    }
    */

    //file_sink->set_level(current_log_level);
    de_vertexwahn::detail::set_sink(file_sink);
}

std::string TestLogger::content() const {
    auto content = detail::read_file(LOG_FILENAME);
    return content;
}

} // namespace de_vertexwahn
