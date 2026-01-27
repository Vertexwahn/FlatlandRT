/*
 *  SPDX-FileCopyrightText: Copyright 2026 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/demangle.hpp"

#include <boost/core/demangle.hpp>

DE_VERTEXWAHN_BEGIN_NAMESPACE

string demangle(const char *name) {
    return boost::core::demangle(name);
}

DE_VERTEXWAHN_END_NAMESPACE
