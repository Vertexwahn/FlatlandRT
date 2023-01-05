/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "boost/algorithm/string.hpp"
#include "boost/predef.h"

#include "gmock/gmock.h"

#include <string>
#include <vector>

TEST(Boost, Algorithm_Split) {
    std::vector<float> values;
    std::string coordinates = "100, 600";
    std::vector<std::string> tokens;
    boost::split(tokens, coordinates, boost::is_any_of(","));

    for(auto token : tokens) {
        float value = std::stof(token);
        values.push_back(value);
    }

    EXPECT_THAT(values[0], 100.f);
    EXPECT_THAT(values[1], 600.f);
}
