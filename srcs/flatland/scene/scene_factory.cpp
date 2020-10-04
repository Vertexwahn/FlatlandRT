/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/scene/scene_factory.h"

FLATLAND_BEGIN_NAMESPACE

const char *SceneFactoryException::what() const throw() {
    return errorMessage_.c_str();
}

SceneFactoryException::~SceneFactoryException() {}

SceneFactory::SceneFactory() {}

SceneFactory::~SceneFactory() {}

SceneFactoryClassDoesNotExist::SceneFactoryClassDoesNotExist(std::string_view className) {
    errorMessage_ = fmt::format("Class with name '{}' does not exist registered",
                                className);
}

SceneFactoryClassAlreadyRegisteredException::SceneFactoryClassAlreadyRegisteredException(
        std::string_view className) {
    errorMessage_ = fmt::format("Class with name '{}' was already registered",
                                className);
}

ReferenceCounted<Shape2f>
SceneFactory::createShape(std::string_view name, const PropertySet &ps, const fs::path& path) const {
    if(creationFunction_.find(std::string(name)) == creationFunction_.end()) {
        throw SceneFactoryClassDoesNotExist(name);
    }

    return creationFunction_.at(std::string(name))(ps,path);
}

void SceneFactory::registerClass(const std::string &className,
                                 std::function<ReferenceCounted<Shape2f>(PropertySet, fs::path)> createObject) {
    if(creationFunction_.find(className) != creationFunction_.end()) {
        throw SceneFactoryClassAlreadyRegisteredException(className);
    }

    creationFunction_[className] = createObject;
}

FLATLAND_END_NAMESPACE