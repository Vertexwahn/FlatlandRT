/*
 *  SPDX-FileCopyrightText: Copyright 2022-2025 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  This source file contains source code copied from pbrt-v4
 *  SPDX-FileCopyrightText: 1998-2016 Matt Pharr, Greg Humphreys, and Wenzel Jakob.
 *  SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once
#ifndef De_Vertexwahn_Math_AxisAlignedBoundingBoxType_e49220cc_5ee9_4ba7_95ca_bbecef080bc5_h
#define De_Vertexwahn_Math_AxisAlignedBoundingBoxType_e49220cc_5ee9_4ba7_95ca_bbecef080bc5_h

#include "math/point.hpp"
#include "math/vector.hpp"

#include <cassert>

DE_VERTEXWAHN_BEGIN_NAMESPACE

template<typename ScalarType, unsigned int Dimension>
struct AxisAlignedBoundingBoxType {
    using Scalar = ScalarType;
    using Point = PointType<ScalarType, Dimension>;
    using Vector = VectorType<ScalarType, Dimension>;

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

    // Copied from PBRTv4 (with minor modifications)
    bool intersect(const Point& origin, const Vector& direction, Scalar t_max, Scalar *hitt) {
        Scalar t0 = 0, t1 = t_max;
        for (size_t i = 0; i < Dimension; ++i) {
            // Update interval for _i_th bounding box slab
            Scalar inverse_ray_direction = Scalar{1} / direction[i];
            Scalar t_near = (min_[i] - origin[i]) * inverse_ray_direction;
            Scalar t_far = (max_[i] - origin[i]) * inverse_ray_direction;
            // Update parametric interval from slab intersection $t$ values
            if (t_near > t_far)
                std::swap(t_near, t_far);
            // Update _tFar_ to ensure robust ray--bounds intersection
            t_far *= 1 + 2 * gamma(3);

            t0 = t_near > t0 ? t_near : t0;
            t1 = t_far < t1 ? t_far : t1;
            if (t0 > t1)
                return false;
        }
        if (hitt) {
            *hitt = t0;
        }
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

    PointType<ScalarType, Dimension> min_;
    PointType<ScalarType, Dimension> max_;
};

template<typename ScalarType>
using AxisAlignedBoundingBox2 = AxisAlignedBoundingBoxType<ScalarType, 2>;
template<typename ScalarType>
using AxisAlignedBoundingBox3 = AxisAlignedBoundingBoxType<ScalarType, 3>;

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

template<typename ScalarType, unsigned int Dimension>
inline bool operator==(const AxisAlignedBoundingBoxType<ScalarType, Dimension> &lhs,
                       const AxisAlignedBoundingBoxType<ScalarType, Dimension> &rhs) {
    return lhs.min_ == rhs.min_ && lhs.max_ == rhs.max_;
}

template<typename ScalarType, unsigned int Dimension>
std::ostream &operator<<(std::ostream &stream, const AxisAlignedBoundingBoxType<ScalarType, Dimension> &aabb) {
    stream << "AABB {" << aabb.min_ << ", " << aabb.max_ << "}";
    return stream;
}

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Math_AxisAlignedBoundingBoxType_e49220cc_5ee9_4ba7_95ca_bbecef080bc5_h
