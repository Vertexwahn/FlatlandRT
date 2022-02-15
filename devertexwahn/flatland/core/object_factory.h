/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Core_ShapeFactory_b184faf0_dc66_451e_a52f_615a4caa7c35_h
#define Flatland_Core_ShapeFactory_b184faf0_dc66_451e_a52f_615a4caa7c35_h

#include "flatland/core/namespace.h"
#include "flatland/core/object.h"
#include "flatland/core/property_set.h"
#include "flatland/core/reference_counted.h"

#include <filesystem>
#include <functional>

FLATLAND_BEGIN_NAMESPACE

class ObjectFactoryException : public std::exception {
public:
    [[nodiscard]]
    const char *what() const throw() override;

    virtual ~ObjectFactoryException() = default;

protected:
    std::string error_message_;
};

class ObjectFactoryClassAlreadyRegisteredException : public ObjectFactoryException {
public:
    explicit ObjectFactoryClassAlreadyRegisteredException(std::string_view className);
};

class ObjectFactoryClassDoesNotExist : public ObjectFactoryException {
public:
    explicit ObjectFactoryClassDoesNotExist(std::string_view className);
};

template<class PropertySetType>
class ObjectFactory {
public:
    ObjectFactory() = default;
    virtual ~ObjectFactory() = default;

    ReferenceCounted<Object> create_instance(std::string_view name, const PropertySetType& ps) const {
        if(creation_function_.find(std::string(name)) == creation_function_.end()) {
            throw ObjectFactoryClassDoesNotExist(name);
        }

        return creation_function_.at(std::string(name))(ps);
    }

    template <typename ObjectType>
    void register_class(const std::string& className) {
        if(creation_function_.find(className) != creation_function_.end()) {
            throw ObjectFactoryClassAlreadyRegisteredException(className);
        }

        std::function<ReferenceCounted<Object>(PropertySetType)> createObject = [](const PropertySetType& ps){
            return make_reference_counted<ObjectType>(ps);
        };

        creation_function_[className] = createObject;
    }

private:
    std::map<std::string, std::function<ReferenceCounted<Object>(PropertySetType)>> creation_function_;
};

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Core_ShapeFactory_b184faf0_dc66_451e_a52f_615a4caa7c35_h
