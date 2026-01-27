/*
 *  SPDX-FileCopyrightText: Copyright 2026 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/shape/triangle_mesh.hpp"

#include "core/object_factory.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

void register_TriangleMesh2f() {
    ObjectFactory<PropertySet>::instance().register_class<TriangleMesh2f>("triangle_mesh");
}

DE_VERTEXWAHN_END_NAMESPACE
