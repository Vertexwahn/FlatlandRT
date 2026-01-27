/*
 *  SPDX-FileCopyrightText: Copyright 2026 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/sampler.hpp"

#include "core/object_factory.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

void register_IndependentSampler() {
    ObjectFactory<PropertySet>::instance().register_class<IndependentSampler>("independent");
}

void register_ConstantSampler() {
    ObjectFactory<PropertySet>::instance().register_class<ConstantSampler>("static");
}

DE_VERTEXWAHN_END_NAMESPACE
