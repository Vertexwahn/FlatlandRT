/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Math_Frame_ddd0adba_4e78_40c5_882e_e8c4ea82c836_h
#define De_Vertexwahn_Math_Frame_ddd0adba_4e78_40c5_882e_e8c4ea82c836_h

#include "math/normal.h"
#include "math/vector.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

// https://graphics.pixar.com/library/OrthonormalB/paper.pdf
template<typename Scalar>
void branchless_onb(const NormalType<3, Scalar> &n, VectorType<3, Scalar> &b1, VectorType<3, Scalar> &b2) {
    using Vector = VectorType<3, Scalar>;

    Scalar sign = copysignf(Scalar{1.0}, n.z());
    const Scalar a = -Scalar{1.0} / (sign + n.z());
    const Scalar b = n.x() * n.y() * a;
    b1 = Vector{Scalar{1.0} + sign * n.x() * n.x() * a, sign * b, -sign * n.x()};
    b2 = Vector{b, sign + n.y() * n.y() * a, -n.y()};
}

// https://graphics.pixar.com/library/OrthonormalB/paper.pdf
template<typename Scalar>
void revised_onb(const NormalType<3, Scalar> &n, VectorType<3, Scalar> &b1, VectorType<3, Scalar> &b2) {
    using Vector = VectorType<3, Scalar>;

    if (n.z() < Scalar{0.}) {
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

template<unsigned int Dimension, typename ScalarType>
struct FrameType {
    using Vector = VectorType<Dimension, ScalarType>;
    using Normal = NormalType<Dimension, ScalarType>;

    Normal normal;
    Vector tangent;

    FrameType() {
    }

    explicit FrameType(const Normal &n, const Vector &t) : normal(n), tangent(t) {
    }

    Vector to_world(const Vector &v) const {
        return v.x() * tangent + v.y() * normal;
    }
};

template<typename ScalarType>
struct FrameType<3, ScalarType> {
    using Vector = VectorType<3, ScalarType>;
    using Normal = NormalType<3, ScalarType>;

    Vector s; // tangent; x
    Vector t; // bitangent; y
    Normal n; // normal; z

    FrameType() {
    }

    FrameType(const Normal3f &n) : n(n) {
        branchless_onb(n, s, t);
    }

    explicit FrameType(const Vector &x, const Vector &y, const Normal &n) : s(x), t(y), n(n) {
    }

    Vector to_world(const Vector &v) const {
        return v.x() * s + v.y() * t + v.z() * Vector(n);
    }
};

template<typename ScalarType>
std::ostream &operator<<(std::ostream &stream, const FrameType<3, ScalarType> &frame) {
    stream << "Frame3 {" << frame.s << ",\n" << frame.t << ",\n" << frame.n << "}";
    return stream;
}

template<typename ScalarType>
using Frame2 = FrameType<2, ScalarType>;
template<typename ScalarType>
using Frame3 = FrameType<3, ScalarType>;

using Frame2f = Frame2<float>;
using Frame2d = Frame2<double>;
using Frame3f = Frame3<float>;
using Frame3d = Frame3<double>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Math_Frame_ddd0adba_4e78_40c5_882e_e8c4ea82c836_h
