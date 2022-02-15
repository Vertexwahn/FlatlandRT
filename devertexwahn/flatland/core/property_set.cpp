/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/core/property_set.h"

FLATLAND_BEGIN_NAMESPACE

PropertyDoesNotExistException::PropertyDoesNotExistException(std::string_view property_name) {
    error_message_ = fmt::format("Property with name '{}' does not exist", property_name);
}

FLATLAND_END_NAMESPACE