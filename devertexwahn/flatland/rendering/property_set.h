/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Rendering_PropertySet_608cdb99_2475_4a1e_86ce_3daa47c29247_h
#define Flatland_Rendering_PropertySet_608cdb99_2475_4a1e_86ce_3daa47c29247_h

#include "flatland/core/namespace.h"
#include "flatland/core/object.h"
#include "flatland/core/property_set.h"
#include "flatland/core/reference_counted.h"
#include "flatland/rendering/property_set_formatter.h"

#include "fmt/core.h"

#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <variant>

FLATLAND_BEGIN_NAMESPACE

using VariantType = std::variant<
        bool,
        int,
        float,
        double,
        char,
        std::string,
        Color3f,
        ReferenceCounted<Object>,
        Point2f,
        Point3f,
        Transform44f,
        Vector2d,
        Vector2f,
        Vector3f,
        Vector3d>;

using PropertySet = PropertySetType<VariantType>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Rendering_PropertySet_608cdb99_2475_4a1e_86ce_3daa47c29247_h
