/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/property_set.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

PropertyDoesNotExistException::PropertyDoesNotExistException(std::string_view property_name) {
    error_message_ = fmt::format("Property with name '{}' does not exist", property_name);
}

DE_VERTEXWAHN_END_NAMESPACE