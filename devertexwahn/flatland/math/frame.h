/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Math_Frame_ddd0adba_4e78_40c5_882e_e8c4ea82c836_h
#define Flatland_Math_Frame_ddd0adba_4e78_40c5_882e_e8c4ea82c836_h

#include "flatland/math/normal.h"
#include "flatland/math/vector.h"

FLATLAND_BEGIN_NAMESPACE

// https://graphics.pixar.com/library/OrthonormalB/paper.pdf
template <typename Scalar>
void revised_onb(const NormalType<3, Scalar>& n, VectorType<3, Scalar>& b1, VectorType<3, Scalar>& b2) {
    using Vector = VectorType<3, Scalar>;

    if (n.z() < 0.) {
        const Scalar a = Scalar{1.0} / (Scalar{1.0} - n.z());
        const Scalar b = n.x() * n.y() * a;
        b1 = Vector(Scalar{1.0} - n.x() * n.x() * a, -b, n.x());
        b2 = Vector(b, n.y() * n.y() * a - Scalar{1.0}, -n.y());
    } else {
        const Scalar a = Scalar{1.0} / (Scalar{1.0} + n.z());
        const Scalar b = -n.x() * n.y() * a;
        b1 = Vector(Scalar{1.0} - n.x() * n.x() * a, b, -n.x());
        b2 = Vector(b, Scalar{1.0} - n.y() * n.y() * a, -n.y());
    }
}

template <unsigned int Dimension, typename ScalarType>
struct FrameType {
    using Vector = VectorType<Dimension, ScalarType>;
    using Normal = NormalType<Dimension, ScalarType>;

    Normal normal;
    Vector tangent;

    FrameType() {
    }

    explicit FrameType(const Normal& n, const Vector& t) : normal(n), tangent(t) {
    }

    Vector to_world(const Vector& v) const {
        return v.x() * tangent + v.y() * normal;
    }
};

template <typename ScalarType>
struct FrameType<3, ScalarType> {
    using Vector = VectorType<3, ScalarType>;
    using Normal = NormalType<3, ScalarType>;

    Vector s; // tangent;
    Vector t; // bitangent;
    Normal n; // normal;

    FrameType() {
    }

    FrameType(const Normal3f& n) : n(n) {
        revised_onb(n, s, t);
    }

    explicit FrameType(const Vector& x, const Vector& y, const Normal& n) : s(x), t(y), n(n) {
    }

    Vector to_world(const Vector& v) const {
        return v.x() * s + v.y() * t + v.z() * Vector(n);
    }
};

template <typename ScalarType>
using Frame2 = FrameType<2, ScalarType>;
template <typename ScalarType>
using Frame3 = FrameType<3, ScalarType>;

using Frame2f = Frame2<float>;
using Frame2d = Frame2<double>;
using Frame3f = Frame3<float>;
using Frame3d = Frame3<double>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Math_Frame_ddd0adba_4e78_40c5_882e_e8c4ea82c836_h
