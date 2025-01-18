/*
 *  SPDX-FileCopyrightText: Copyright 2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/logging.hpp"

#include "cpuinfo.h"

#include <iostream>

int main() {
    std::cout << "CPU Info" << std::endl;

    bool result = cpuinfo_initialize();
    if(!result) {
        return 2;
    }

    LOG_INFO("Running on {} CPU", cpuinfo_get_package(0)->name);

    return 0;
}
