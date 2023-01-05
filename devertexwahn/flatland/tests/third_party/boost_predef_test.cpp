/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "boost/predef.h"

#include "gmock/gmock.h"

#include <string>

TEST(Boost, Predef) {
    std::string arch = BOOST_ARCH_ARM_NAME;
}