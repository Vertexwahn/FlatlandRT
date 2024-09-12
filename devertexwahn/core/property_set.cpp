/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/property_set.hpp"

#include "fmt/core.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

PropertyDoesNotExistException::PropertyDoesNotExistException(std::string_view property_name) {
    error_message_ = fmt::format("Property with name '{}' does not exist", property_name);
}

DE_VERTEXWAHN_END_NAMESPACE
