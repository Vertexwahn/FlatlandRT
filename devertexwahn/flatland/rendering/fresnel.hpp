/*
 *  SPDX-FileCopyrightText: Copyright 2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_Rendering_fresnel_ba618a3b_8405_49fb_bd9c_36d9474e98de_h
#define De_Vertexwahn_Flatland_Rendering_fresnel_ba618a3b_8405_49fb_bd9c_36d9474e98de_h

#include "core/namespace.hpp"
#include "math/vector.hpp"

DE_VERTEXWAHN_BEGIN_NAMESPACE

// compute reflection direction
Vector3f reflect(const Vector3f& v, const Vector3f& n) {
    return -v + 2.0f * v.dot(n) * n;
}

// compute refracted direction
bool refract(const Vector3f& v, const Vector3f& n, float iorI, float iorT,
                    Vector3f& t) {
    const Vector3f t_h = -iorI / iorT * (v - v.dot(n) * n);
    // total reflection
    if (t_h.norm() > 1.0f) {
        return false;
    }
    const Vector3f t_p = -std::sqrt(std::max(1.0f - t_h.norm(), 0.0f)) * n;
    t = t_h + t_p;
    return true;
}

/*
// schlick approximation of fresnel reflectance
float fresnel(float cosThetaI, float iorI, float iorT) {
    const float f0 =
            (iorI - iorT) * (iorI - iorT) / ((iorI + iorT) * (iorI + iorT));
    const auto pow5 = [](float x) { return x * x * x * x * x; };
    return f0 + (1.0f - f0) * pow5(std::max(1.0f - std::abs(cosThetaI), 0.0f));
}
*/
DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Rendering_fresnel_ba618a3b_8405_49fb_bd9c_36d9474e98de_h
