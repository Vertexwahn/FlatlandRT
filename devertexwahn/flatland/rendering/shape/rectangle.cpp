/*
 *  SPDX-FileCopyrightText: Copyright 2026 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/shape/rectangle.hpp"

#include "core/object_factory.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

void register_Rectangle2f() {
    ObjectFactory<PropertySet>::instance().register_class<Rectangle2f>("rectangle");
}

DE_VERTEXWAHN_END_NAMESPACE
