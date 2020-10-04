/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_ShapeFactory_b184faf0_dc66_451e_a52f_615a4caa7c35_h
#define Flatland_ShapeFactory_b184faf0_dc66_451e_a52f_615a4caa7c35_h

#include "flatland/core/property_set.h"
#include "flatland/core/reference_counted.h"
#include "flatland/scene/shape/shape.h"
#include "flatland/core/namespace.h"
#include "flatland/core/filesystem.h"

FLATLAND_BEGIN_NAMESPACE

class SceneFactoryException : public std::exception {
public:
    const char *what() const throw() override;

    virtual ~SceneFactoryException();

protected:
    std::string errorMessage_;
};

class SceneFactoryClassAlreadyRegisteredException : public SceneFactoryException {
public:
    explicit SceneFactoryClassAlreadyRegisteredException(std::string_view className);
};

class SceneFactoryClassDoesNotExist : public SceneFactoryException {
public:
    explicit SceneFactoryClassDoesNotExist(std::string_view className);
};

class SceneFactory {
public:
    SceneFactory();
    virtual ~SceneFactory();
    ReferenceCounted<Shape2f> createShape(std::string_view name, const PropertySet& ps, const fs::path& path) const;
    void registerClass(const std::string& className, std::function<ReferenceCounted<Shape2f>(PropertySet, fs::path)> createObject);

private:
    std::map<std::string, std::function<ReferenceCounted<Shape2f>(PropertySet, fs::path)>> creationFunction_;
};

FLATLAND_END_NAMESPACE

#endif // end define BlueFramework_Core_ShapeFactory_b184faf0_dc66_451e_a52f_615a4caa7c35_h
