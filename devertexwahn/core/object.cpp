/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/object.hpp"

#include <stdexcept>

DE_VERTEXWAHN_BEGIN_NAMESPACE

void Object::add_child(Object* child) {
    throw std::runtime_error("This object type does not support children.");
}

string Object::to_string() const {
    return "An object. Implement the to_string method to improve this description.";
}

DE_VERTEXWAHN_END_NAMESPACE
