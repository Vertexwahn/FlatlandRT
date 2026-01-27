/*
 *  SPDX-FileCopyrightText: Copyright 2026 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/shape/polygon.hpp"

#include "core/object_factory.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

void register_Polygon2f() {
    ObjectFactory<PropertySet>::instance().register_class<Polygon2f>("polygon");
}

DE_VERTEXWAHN_END_NAMESPACE
