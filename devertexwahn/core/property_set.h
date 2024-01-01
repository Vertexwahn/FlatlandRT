/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#ifndef De_Vertexwahn_Core_PropertySet_846e76bd_8e54_47a4_940b_a8d5c312da23_h
#define De_Vertexwahn_Core_PropertySet_846e76bd_8e54_47a4_940b_a8d5c312da23_h

#include "core/namespace.h"

#include "fmt/core.h"

#include <cstddef>
#include <exception>
#include <initializer_list>
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <variant>

DE_VERTEXWAHN_BEGIN_NAMESPACE

class PropertyException : public std::exception {
public:
    [[nodiscard]]
    const char *what() const throw() override {
        return error_message_.c_str();
    }

    virtual ~PropertyException() = default;

    // virtual ~PropertyException() noexcept {} // Todo: Find out what is better here

protected:
    std::string error_message_;
};

class PropertyDoesNotExistException : public PropertyException {
public:
    explicit PropertyDoesNotExistException(std::string_view property_name);
};

class PropertyDoesAlreadyExistException : public PropertyException {
public:
    template<typename ValueType>
    explicit PropertyDoesAlreadyExistException(std::string_view property_name, const ValueType &value) {
        error_message_ =
                fmt::format("Property with name '{}' does already exist and its value is '{}'", property_name,
                            value);
    }
};

// Todo: Should also contain which type was requested and what type it actually is
class PropertyWithWrongValueTypeRequested : public PropertyException {
public:
    explicit PropertyWithWrongValueTypeRequested(std::string_view property_name) {
        error_message_ =
                fmt::format("Existing Property with name '{}' was requested but wrong type was specified",
                            property_name);
    }
};

// The following template function was copied from an answer provided on Stack Overflow.
// See here: https://stackoverflow.com/questions/46091671/c-template-recursive-to-check-type-in-stdtuple
// The original author (Vittorio Romeo) was asked and gave his approval ("feel free to use and modify at will.").
template<typename VariantType, typename T, std::size_t index = 0>
constexpr std::size_t variant_index() {
    if constexpr (index == std::variant_size_v<VariantType>) {
        return index;
    } else if constexpr (std::is_same_v<std::variant_alternative_t<index, VariantType>, T>) {
        return index;
    } else {
        return variant_index<VariantType, T, index + 1>();
    }
}

template<class VariantType>
class PropertySetType {
public:
    using MapElementType = std::pair<std::string, VariantType>;

public:
    PropertySetType() = default;

    PropertySetType(const std::initializer_list<MapElementType> &list) {
        for (const auto &element : list) {
            property_name_to_value_.emplace(element);
        }
    }

    template<typename ValueType>
    void add_property(const std::string &name, const ValueType &value) {
        if (has_property(name)) {
            throw PropertyDoesAlreadyExistException(name, value);
        } else {
            property_name_to_value_[name] = value;
        }
    }

    template<typename ValueType>
    [[nodiscard]]
    const ValueType &get_property(const std::string &name) const {
        if (has_property(name)) {
            const VariantType &vt = property_name_to_value_.at(name);
            if (vt.index() != variant_index<VariantType, ValueType>()) {
                throw PropertyWithWrongValueTypeRequested(name.c_str());
            }
            const auto &value = std::get<ValueType>(vt);

            return value;
        } else {
            throw PropertyDoesNotExistException(name);
        }
    }

    // A default value can be provided in the case a property does not exist

    template<typename ValueType>
    [[nodiscard]]
    const ValueType &get_property(const std::string &name, const ValueType &default_value) const {
        if (has_property(name)) {
            return get_property<ValueType>(name);
        } else {
            return default_value;
        }
    }

    [[nodiscard]]
    bool has_property(const std::string &name) const {
        return property_name_to_value_.find(name) != property_name_to_value_.end();
    }

    /*
     * Sets the value of an existing property. If the property is not existing an exception it thrown.
     */
    template<typename ValueType>
    void set_property(const std::string &name, const ValueType &value) {
        if (has_property(name)) {
            VariantType &vt = property_name_to_value_.at(name);
            if (vt.index() != variant_index<VariantType, ValueType>()) {
                throw PropertyWithWrongValueTypeRequested(name.c_str());
            }
            auto &property = std::get<ValueType>(vt);

            property = value;
        } else {
            throw PropertyDoesNotExistException(name);
        }
    }

private:
    using MapType = std::map<std::string, VariantType>;

    MapType property_name_to_value_;
};

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Core_PropertySet_846e76bd_8e54_47a4_940b_a8d5c312da23_h
