#include "math/refract.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

// Copied from pbrt, Version 3 with minor modifications (see README.md for license information)
// Source: https://github.com/mmp/pbrt-v3/blob/0aa90372efce97df39cffcaa33b276d7d0772d30/src/core/geometry.h#L1213
// http://www.pbr-book.org/3ed-2018/Reflection_Models/Specular_Reflection_and_Transmission.html
Normal2f face_forward(const Normal2f &n, const Vector2f &v) {
    return (n.dot(v) < 0.f) ? Normal2f(-n.x(),n.y()) : n;
}

DE_VERTEXWAHN_END_NAMESPACE
