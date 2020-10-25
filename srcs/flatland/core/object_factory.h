/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_ShapeFactory_b184faf0_dc66_451e_a52f_615a4caa7c35_h
#define Flatland_ShapeFactory_b184faf0_dc66_451e_a52f_615a4caa7c35_h

#include "srcs/flatland/core/property_set.h"
#include "srcs/flatland/core/reference_counted.h"
#include "srcs/flatland/scene/shape/shape.h"
#include "srcs/flatland/integrator/integrator.h"
#include "srcs/flatland/core/namespace.h"

#include <filesystem>

FLATLAND_BEGIN_NAMESPACE

class ObjectFactoryException : public std::exception {
public:
    const char *what() const throw() override;

    virtual ~ObjectFactoryException();

protected:
    std::string errorMessage_;
};

class ObjectFactoryClassAlreadyRegisteredException : public ObjectFactoryException {
public:
    explicit ObjectFactoryClassAlreadyRegisteredException(std::string_view className);
};

class ObjectFactoryClassDoesNotExist : public ObjectFactoryException {
public:
    explicit ObjectFactoryClassDoesNotExist(std::string_view className);
};

class ObjectFactory {
public:
    ObjectFactory();
    virtual ~ObjectFactory();

    ReferenceCounted<Object> createInstance(std::string_view name, const PropertySet& ps) const;

    template <typename ObjectType>
    void registerClass(const std::string& className) {
        if(creationFunction_.find(className) != creationFunction_.end()) {
            throw ObjectFactoryClassAlreadyRegisteredException(className);
        }

        std::function<ReferenceCounted<Object>(PropertySet)> createObject = [](const PropertySet& ps){
            return makeReferenceCounted<ObjectType>(ps);
        };

        creationFunction_[className] = createObject;
    }

private:
    std::map<std::string, std::function<ReferenceCounted<Object>(PropertySet)>> creationFunction_;
};

FLATLAND_END_NAMESPACE

#endif // end define BlueFramework_Core_ShapeFactory_b184faf0_dc66_451e_a52f_615a4caa7c35_h
