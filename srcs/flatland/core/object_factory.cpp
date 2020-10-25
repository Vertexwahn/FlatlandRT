/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/core/object_factory.h"

FLATLAND_BEGIN_NAMESPACE

const char *ObjectFactoryException::what() const throw() {
    return errorMessage_.c_str();
}

ObjectFactoryException::~ObjectFactoryException() {}

ObjectFactoryClassDoesNotExist::ObjectFactoryClassDoesNotExist(std::string_view className) {
    errorMessage_ = fmt::format("Class with name '{}' does not exist registered",
                                className);
}

ObjectFactoryClassAlreadyRegisteredException::ObjectFactoryClassAlreadyRegisteredException(
        std::string_view className) {
    errorMessage_ = fmt::format("Class with name '{}' was already registered",
                                className);
}

ObjectFactory::ObjectFactory() {}

ObjectFactory::~ObjectFactory() {}

ReferenceCounted<Object>
ObjectFactory::createInstance(std::string_view name, const PropertySet &ps) const {
    if(creationFunction_.find(std::string(name)) == creationFunction_.end()) {
        throw ObjectFactoryClassDoesNotExist(name);
    }

    return creationFunction_.at(std::string(name))(ps);
}

FLATLAND_END_NAMESPACE