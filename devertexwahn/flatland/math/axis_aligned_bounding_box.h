/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  This source file contains source code copied from pbrt-v3
 *  SPDX-FileCopyrightText: 1998-2016 Matt Pharr, Greg Humphreys, and Wenzel Jakob.
 *  SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once
#ifndef Flatland_Math_Bounds_e49220cc_5ee9_4ba7_95ca_bbecef080bc5_h
#define Flatland_Math_Bounds_e49220cc_5ee9_4ba7_95ca_bbecef080bc5_h

#include "flatland/math/point.h"
#include "flatland/math/vector.h"

FLATLAND_BEGIN_NAMESPACE

template<unsigned int Dimension, typename ScalarType>
struct AxisAlignedBoundingBoxType {
    using Scalar = ScalarType;
    using Point = PointType<Dimension, ScalarType>;
    using Vector = VectorType<Dimension, ScalarType>;

    Scalar width() const {
        Scalar width = max_.x() - min_.x();
        assert(width >= 0);
        return width;
    }

    Scalar height() const {
        Scalar height = max_.y() - min_.y();
        assert(height >= 0);
        return height;
    }

    Scalar depth() const {
        Scalar depth = max_.z() - min_.z();
        assert(depth >= 0);
        return depth;
    }

    Vector size() const {
        Vector size;
        for (unsigned int i = 0; i < Dimension; ++i) {
            size[i] = max_[i] - min_[i];
            assert(size[i] >= 0);
        }
        return size;
    }

    bool valid() const {
        return min_.x() <= max_.x() &&
               min_.y() <= max_.y();
    }

    // Copied from PBRTv4
    static constexpr Scalar MachineEpsilon = std::numeric_limits<Scalar>::epsilon() * 0.5;

    // Copied from PBRTv4
    inline constexpr Scalar gamma(int n) const {
        return (n * MachineEpsilon) / (1 - n * MachineEpsilon);
    }

    // Copied from PBRTv4
    bool intersect(Point o, Vector d, Scalar tMax, Scalar *hitt) {
        Scalar t0 = 0, t1 = tMax;
        for (size_t i = 0; i < Dimension; ++i) {
            // Update interval for _i_th bounding box slab
            Scalar invRayDir = Scalar{1} / d[i];
            Scalar tNear = (min_[i] - o[i]) * invRayDir;
            Scalar tFar = (max_[i] - o[i]) * invRayDir;
            // Update parametric interval from slab intersection $t$ values
            if (tNear > tFar)
                std::swap(tNear, tFar);
            // Update _tFar_ to ensure robust ray--bounds intersection
            tFar *= 1 + 2 * gamma(3);

            t0 = tNear > t0 ? tNear : t0;
            t1 = tFar < t1 ? tFar : t1;
            if (t0 > t1)
                return false;
        }
        if (hitt)
            *hitt = t0;
        return true;
    }

    bool contains(Point p) {
        for (size_t i = 0; i < Dimension; ++i) {
            if (p[i] < min_[i]) {
                return false;
            }
            if (p[i] >= max_[i]) {
                return false;
            }
        }

        return true;
    }

    PointType <Dimension, ScalarType> min_;
    PointType <Dimension, ScalarType> max_;
};

template<typename ScalarType>
using AxisAlignedBoundingBox2 = AxisAlignedBoundingBoxType<2, ScalarType>;
template<typename ScalarType>
using AxisAlignedBoundingBox3 = AxisAlignedBoundingBoxType<3, ScalarType>;

using AxisAlignedBoundingBox2i = AxisAlignedBoundingBox2<int>;
using AxisAlignedBoundingBox2f = AxisAlignedBoundingBox2<float>;
using AxisAlignedBoundingBox2d = AxisAlignedBoundingBox2<double>;

using AxisAlignedBoundingBox3i = AxisAlignedBoundingBox3<int>;
using AxisAlignedBoundingBox3f = AxisAlignedBoundingBox3<float>;
using AxisAlignedBoundingBox3d = AxisAlignedBoundingBox3<double>;

// following function was copied from PBRTv4 and slightly modified
template<typename T>
inline bool overlaps(const AxisAlignedBoundingBox2<T> &ba, const AxisAlignedBoundingBox2<T> &bb) {
    assert(ba.valid());
    assert(bb.valid());
    bool x = (ba.max_.x() >= bb.min_.x()) && (ba.min_.x() <= bb.max_.x());
    bool y = (ba.max_.y() >= bb.min_.y()) && (ba.min_.y() <= bb.max_.y());
    return (x && y);
}

template<typename T>
inline bool overlaps(const AxisAlignedBoundingBox3<T> &ba, const AxisAlignedBoundingBox3<T> &bb) {
    assert(ba.valid());
    assert(bb.valid());
    bool x = (ba.max_.x() >= bb.min_.x()) && (ba.min_.x() <= bb.max_.x());
    bool y = (ba.max_.y() >= bb.min_.y()) && (ba.min_.y() <= bb.max_.y());
    bool z = (ba.max_.z() >= bb.min_.z()) && (ba.min_.z() <= bb.max_.z());
    return (x && y && z);
}

// following function was copied from PBRTv4 and slightly modified
template<typename T>
inline AxisAlignedBoundingBox2<T>
union_aabb(const AxisAlignedBoundingBox2<T> &b1, const AxisAlignedBoundingBox2<T> &b2) {
    assert(b1.valid());
    assert(b2.valid());
    AxisAlignedBoundingBox2<T> ret;
    ret.min_.x() = std::min(b1.min_.x(), b2.min_.x());
    ret.min_.y() = std::min(b1.min_.y(), b2.min_.y());
    ret.max_.x() = std::max(b1.max_.x(), b2.max_.x());
    ret.max_.y() = std::max(b1.max_.y(), b2.max_.y());
    return ret;
}

template<typename T>
inline AxisAlignedBoundingBox3<T>
union_aabb(const AxisAlignedBoundingBox3<T> &b1, const AxisAlignedBoundingBox3<T> &b2) {
    assert(b1.valid());
    assert(b2.valid());
    AxisAlignedBoundingBox3<T> ret;
    ret.min_.x() = std::min(b1.min_.x(), b2.min_.x());
    ret.min_.y() = std::min(b1.min_.y(), b2.min_.y());
    ret.min_.z() = std::min(b1.min_.z(), b2.min_.z());
    ret.max_.x() = std::max(b1.max_.x(), b2.max_.x());
    ret.max_.y() = std::max(b1.max_.y(), b2.max_.y());
    ret.max_.z() = std::max(b1.max_.z(), b2.max_.z());
    return ret;
}

template<unsigned int Dimension, typename ScalarType>
inline bool operator==(const AxisAlignedBoundingBoxType<Dimension, ScalarType> &lhs,
                       const AxisAlignedBoundingBoxType<Dimension, ScalarType> &rhs) {
    return lhs.min_ == rhs.min_ && lhs.max_ == rhs.max_;
}

template<unsigned int Dimension, typename ScalarType>
std::ostream &operator<<(std::ostream &stream, const AxisAlignedBoundingBoxType<Dimension, ScalarType> &aabb) {
    stream << "AABB {" << aabb.min_ << ", " << aabb.max_ << "}";
    return stream;
}

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Math_Bounds_e49220cc_5ee9_4ba7_95ca_bbecef080bc5_h
