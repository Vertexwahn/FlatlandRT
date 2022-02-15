/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/core/object.h"

FLATLAND_BEGIN_NAMESPACE

std::string Object::to_string() const {
    return "A object. Implement to_string method to improve description.";
}

FLATLAND_END_NAMESPACE
