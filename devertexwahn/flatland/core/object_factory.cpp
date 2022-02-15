/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/core/object_factory.h"

FLATLAND_BEGIN_NAMESPACE

const char *ObjectFactoryException::what() const throw() {
    return error_message_.c_str();
}

ObjectFactoryClassDoesNotExist::ObjectFactoryClassDoesNotExist(std::string_view className) {
    error_message_ = fmt::format("Class with name '{}' does not exist registered",
                                 className);
}

ObjectFactoryClassAlreadyRegisteredException::ObjectFactoryClassAlreadyRegisteredException(
        std::string_view className) {
    error_message_ = fmt::format("Class with name '{}' was already registered",
                                 className);
}

FLATLAND_END_NAMESPACE