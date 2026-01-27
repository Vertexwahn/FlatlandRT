/*
 *  SPDX-FileCopyrightText: Copyright 2026 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/namespace.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

// Intersector
void register_BruteForceIntersector2f();
void register_QuadtreeIntersector2f();

// Integrator
void register_AmbientOcclusion2f();
void register_PathMirrorReflection2f();
void register_PathSpecularTransmission();

// Shapes
void register_Disk2f();
void register_Polygon2f();
void register_Rectangle2f();
void register_TriangleMesh2f();

// BSDFs
void register_SvgMaterial();

// Samplers
void register_IndependentSampler();

void register_flatland_plugins() {
    // Intersector
    register_BruteForceIntersector2f();
    register_QuadtreeIntersector2f();

    // Integrator
    register_AmbientOcclusion2f();
    register_PathMirrorReflection2f();
    register_PathSpecularTransmission();

    // Shapes
    register_Disk2f();
    register_Polygon2f();
    register_Rectangle2f();
    register_TriangleMesh2f();

    // BSDFs
    register_SvgMaterial();

    // Samplers
    register_IndependentSampler();
}

DE_VERTEXWAHN_END_NAMESPACE
