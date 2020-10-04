/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "property_set.h"

FLATLAND_BEGIN_NAMESPACE

PropertyDoesNotExistException::PropertyDoesNotExistException(std::string_view propertyName) {
    std::stringstream ss;
    ss << "Property with name '" << propertyName << "' does not exist";
    errorMessage_ = ss.str();
}

FLATLAND_END_NAMESPACE