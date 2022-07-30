/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/core/exception.h"

FLATLAND_BEGIN_NAMESPACE

FlatlandException::FlatlandException(std::string_view message) : error_message_(message) {
}

const char *FlatlandException::what() const throw() {
    return error_message_.c_str();
}

FlatlandException::~FlatlandException() {}

FLATLAND_END_NAMESPACE