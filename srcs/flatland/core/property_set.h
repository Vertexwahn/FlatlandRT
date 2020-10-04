/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_PropertySet_608cdb99_2475_4a1e_86ce_3daa47c29247_h
#define Flatland_PropertySet_608cdb99_2475_4a1e_86ce_3daa47c29247_h

#include "flatland/core/color.h"
#include "flatland/math/util.h"
#include "flatland/math/transform.h"
#include "flatland/core/namespace.h"

#include <fmt/core.h>
#include <fmt/format.h>

#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <variant>

template<>
struct fmt::formatter<Flatland::Vector2f> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const Flatland::Vector2f &v, FormatContext &ctx) {
        return format_to(ctx.out(), "({}, {})", v.x(), v.y());
    }
};

template<>
struct fmt::formatter<Flatland::Vector2d> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const Flatland::Vector2d &v, FormatContext &ctx) {
        return format_to(ctx.out(), "({}, {})", v.x(), v.y());
    }
};

template<>
struct fmt::formatter<Flatland::Point2f> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const Flatland::Point2f &v, FormatContext &ctx) {
        return format_to(ctx.out(), "({}, {})", v.x(), v.y());
    }
};

template<>
struct fmt::formatter<Flatland::Point2d> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const Flatland::Point2d &v, FormatContext &ctx) {
        return format_to(ctx.out(), "({}, {})", v.x(), v.y());
    }
};

template<>
struct fmt::formatter<Flatland::Color3f> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const Flatland::Color3f &c, FormatContext &ctx) {
        return format_to(ctx.out(), "({}, {}, {})", c.red(), c.green(), c.blue());
    }
};

template<>
struct fmt::formatter<Flatland::Transform44f> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const Flatland::Transform44f &c, FormatContext &ctx) {
        return format_to(ctx.out(), "({})", c.getMatrix()(0, 0));
    }
};

FLATLAND_BEGIN_NAMESPACE

class PropertyException : public std::exception {
public:
    const char *what() const throw() override {
        return errorMessage_.c_str();
    }

    virtual ~PropertyException() {}

protected:
    std::string errorMessage_;
};

class PropertyDoesNotExistException : public PropertyException {
public:
    explicit PropertyDoesNotExistException(std::string_view propertyName);
};

class PropertyDoesAlreadyExistException : public PropertyException {
public:
    template<typename ValueType>
    explicit PropertyDoesAlreadyExistException(std::string_view propertyName, const ValueType &value) {
        errorMessage_ = fmt::format("Property with name '{}' does already exist and its value is '{}'",
                                    propertyName, value);
    }
};

// Todo: Should also contain which type was requested and what type it actually is
class PropertyWithWrongValueTypeRequested : public PropertyException {
public:
    explicit PropertyWithWrongValueTypeRequested(std::string_view propertyName) {
        errorMessage_ = fmt::format("Existing Property with name '{}' was requested but wrong type was specified",
                                    propertyName);
    }
};

// The following template function was copied from an answer provided on Stack Overflow.
// See here: https://stackoverflow.com/questions/46091671/c-template-recursive-to-check-type-in-stdtuple
// The original author (Vittorio Romeo) was asked and gave his approval.
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

class PropertySet {
public:
    template<typename ValueType>
    void addProperty(const std::string &name, const ValueType &value) {
        if (hasProperty(name)) {
            throw PropertyDoesAlreadyExistException(name, value);
        } else {
            propertyNameToValue_[name] = value;
        }
    }

    template<typename ValueType>
    const ValueType &getProperty(const std::string &name) const {
        if (hasProperty(name)) {
            const VariantType &vt = propertyNameToValue_.at(name);
            if (vt.index() != variant_index<VariantType, ValueType>()) {
                throw PropertyWithWrongValueTypeRequested(name.c_str());
            }
            const auto &value = std::get<ValueType>(vt);

            return value;
        } else {
            throw PropertyDoesNotExistException(name);
        }
    }

    // A defaultValue can be provided in the case a property does not exist
    template<typename ValueType>
    const ValueType &getProperty(const std::string &name,
                                 const ValueType &defaultValue) const {
        if (hasProperty(name)) {
            return getProperty<ValueType>(name);
        } else {
            return defaultValue;
        }
    }

    bool hasProperty(const std::string &name) const {
        return propertyNameToValue_.find(name) != propertyNameToValue_.end();
    }

private:
    using VariantType = std::variant<bool,
            int,
            float,
            double,
            char,
            std::string,
            Vector2f,
            Vector2d,
            Point2f,
            //Point2d,
            Color3f,
            Transform44f>;

    using MapType = std::map<std::string, VariantType>;

    MapType propertyNameToValue_;
};

FLATLAND_END_NAMESPACE

#endif // end define Flatland_PropertySet_608cdb99_2475_4a1e_86ce_3daa47c29247_h
