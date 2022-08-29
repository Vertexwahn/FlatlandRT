/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "boost/predef.h"

#include "include/gmock/gmock.h"

#include <string>

TEST(Boost, Predef) {
    std::string arch = BOOST_ARCH_ARM_NAME;
}