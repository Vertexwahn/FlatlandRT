#pragma once
#ifndef Flatland_Math_refraction_8ce77914_917c_4236_a751_491f3fe26041_h
#define Flatland_Math_refraction_8ce77914_917c_4236_a751_491f3fe26041_h

#include "flatland/math/normal.h"
#include "flatland/math/vector.h"
#include "flatland/core/namespace.h"

FLATLAND_BEGIN_NAMESPACE

// Copied from pbrt, Version 3 with minor modifications (see README.md for license information)
// Source: https://github.com/mmp/pbrt-v3/blob/d3d6cc54291f1467ad4ad3c70b89afb1a87379f6/src/core/reflection.h#L97
template <typename Vector, typename Normal, typename ScalarType>
bool refract(const Vector &wi, const Normal &n, const ScalarType eta, Vector &wo) {
    typedef ScalarType Float;

    Float cosThetaI = n.dot(wi);
    Float sin2ThetaI = std::max(0.f, 1.f - cosThetaI * cosThetaI);
    Float sin2ThetaT = eta * eta * sin2ThetaI;

    if (sin2ThetaT >= 1) return false;
    Float cosThetaT = std::sqrt(1 - sin2ThetaT);
    wo = eta * -wi + (eta * cosThetaI - cosThetaT) * Vector2f(n);
    return true;
}

// Copied from pbrt, Version 3 with minor modifications (see README.md for license information)
// Source: https://github.com/mmp/pbrt-v3/blob/0aa90372efce97df39cffcaa33b276d7d0772d30/src/core/geometry.h#L1213
// http://www.pbr-book.org/3ed-2018/Reflection_Models/Specular_Reflection_and_Transmission.html
Normal2f faceforward(const Normal2f &n, const Vector2f &v);

template <typename ScalarType>
inline Normal3<ScalarType> face_forward(const Normal3<ScalarType> &n, const Normal3<ScalarType> &n2) {
    return (n.dot(n2) < ScalarType{0.}) ? -n : n;
}

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Math_refraction_8ce77914_917c_4236_a751_491f3fe26041_h
