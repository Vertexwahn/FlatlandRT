/*
 *  SPDX-FileCopyrightText: Copyright 2026 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/integrator/ambient_occlusion.hpp"

#include "core/object_factory.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

void register_AmbientOcclusion2f() {
    ObjectFactory<PropertySet>::instance().register_class<AmbientOcclusion2f>("ambient_occlusion");
}

DE_VERTEXWAHN_END_NAMESPACE
