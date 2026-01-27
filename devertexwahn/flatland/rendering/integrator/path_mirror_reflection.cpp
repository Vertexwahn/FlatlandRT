/*
 *  SPDX-FileCopyrightText: Copyright 2026 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/integrator/path_mirror_reflection.hpp"

#include "core/object_factory.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

void register_PathMirrorReflection2f() {
    ObjectFactory<PropertySet>::instance().register_class<PathMirrorReflection2f>("path_mirror_reflection");
}

DE_VERTEXWAHN_END_NAMESPACE
