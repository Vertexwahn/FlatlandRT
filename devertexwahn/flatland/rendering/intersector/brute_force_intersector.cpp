/*
 *  SPDX-FileCopyrightText: Copyright 2026 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/intersector/brute_force_intersector.hpp"

#include "core/namespace.hpp"
#include "core/object_factory.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

void register_BruteForceIntersector2f() {
    ObjectFactory<PropertySet>::instance().register_class<BruteForceIntersector2f>("brute_force");
}

void register_BruteForceIntersector3f() {
    ObjectFactory<PropertySet>::instance().register_class<BruteForceIntersector3f>("brute_force");
}

DE_VERTEXWAHN_END_NAMESPACE
