/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_Rendering_PropertySet_608cdb99_2475_4a1e_86ce_3daa47c29247_h
#define De_Vertexwahn_Flatland_Rendering_PropertySet_608cdb99_2475_4a1e_86ce_3daa47c29247_h

#include "flatland/rendering/property_set_formatter.h"

#include "imaging/color.h"

#include "math/point.h"
#include "math/transform.h"
#include "math/vector.h"

#include "core/namespace.h"
#include "core/object.h"
#include "core/property_set.h"
#include "core/reference_counted.h"

#include <string>
#include <variant>

DE_VERTEXWAHN_BEGIN_NAMESPACE

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

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Rendering_PropertySet_608cdb99_2475_4a1e_86ce_3daa47c29247_h
