/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Math_Transform_5c51bd58_f3a5_4ce3_9921_c4906c57beb2_h
#define De_Vertexwahn_Math_Transform_5c51bd58_f3a5_4ce3_9921_c4906c57beb2_h

#include "math/matrix.h"
#include "math/normal.h"
#include "math/point.h"
#include "math/ray.h"
#include "math/vector.h"

#include <iostream>

DE_VERTEXWAHN_BEGIN_NAMESPACE

template <typename ScalarType>
class Transform44Type {
public:
    using Matrix = Matrix44<ScalarType>;
    using Vector = VectorType<ScalarType, 2>;
    using Point = PointType<ScalarType, 2>;
    using Normal = NormalType<ScalarType, 2>;
    using Scalar = ScalarType;
    using Ray = RayType<ScalarType, 2>;

	Transform44Type() {
        transform_ = inverse_ = Matrix::Identity();
	}

    Transform44Type(const Matrix& matrix) {
        transform_ = matrix;
        inverse_ = matrix.inverse();
    }

    Transform44Type(const Matrix& matrix, const Matrix& inverse) {
        transform_ = matrix;
        inverse_ = inverse;

        assert(inverse_.isApprox(matrix.inverse()));
    }

    VectorType<ScalarType, 2> operator*(const VectorType<ScalarType, 2>& v) const {
        Vector4<ScalarType> v4{v.x(), v.y(), Scalar{0.0}, Scalar{0.0}};
        Vector4<ScalarType> vt = transform_ * v4;
        return VectorType<ScalarType, 2>{vt.x(), vt.y()};
    }

    VectorType<ScalarType, 3> operator*(const VectorType<ScalarType, 3>& v) const {
        Vector4<ScalarType> v4{v.x(), v.y(), v.z(), Scalar{0.0}};
        Vector4<ScalarType> vt = transform_ * v4;
        return VectorType<ScalarType, 3>{vt.x(), vt.y(), vt.z()};
    }

	PointType<ScalarType, 2> operator*(const PointType<ScalarType, 2>& p) const {
        Point4<ScalarType> p4{p.x(), p.y(), Scalar{0.0}, Scalar{1.0}};
        Point4<ScalarType> pt = transform_ * p4;
		return PointType<ScalarType, 2>{pt.x(), pt.y()};
	}

    PointType<ScalarType, 3> operator*(const PointType<ScalarType, 3>& p) const {
        Point4<ScalarType> p4{p.x(), p.y(), p.z(), Scalar{1.0}};
        Point4<ScalarType> pt = transform_ * p4;
        return PointType<ScalarType, 3>{pt.x()/pt.w(), pt.y()/pt.w(), pt.z()/pt.w()};
    }

    PointType<ScalarType, 4> operator*(const PointType<ScalarType, 4>& p) const {
        return transform_ * p;
    }

    VectorType<ScalarType, 4> operator*(const VectorType<ScalarType, 4>& p) const {
        return transform_ * p;
    }

    Normal operator*(const Normal& normal) const {
        Vector4<ScalarType> v{normal.x(), normal.y(), Scalar{0.0}, Scalar{0.0}};
        auto vt = transform_.inverse().transpose() * v;
        return Normal{vt.x(), vt.y()};
    }

    NormalType<ScalarType, 3> operator*(const NormalType<ScalarType, 3>& normal) const {
        Vector4<ScalarType> v{normal.x(), normal.y(), normal.z(), Scalar{0.0}};
        auto vt = transform_.inverse().transpose() * v;
        NormalType<ScalarType, 3> n{vt.x(), vt.y(), vt.z()};
        return n.normalized();
    }

    Ray operator*(const Ray& ray_) const {
        Vector4<ScalarType> o{ray_.origin.x(), ray_.origin.y(), ScalarType{0.0}, ScalarType{1.0}};
        o = transform_ * o;
        Vector3<ScalarType> d{ray_.direction.x(), ray_.direction.y(), ScalarType{0.0}};
        d = transform_.template topLeftCorner<3,3>() * d;
	    return Ray{
            Point{o.x(), o.y()},
            Vector{d.x(), d.y()},
            ray_.min_t,
            ray_.max_t
	    };
    }

    RayType<ScalarType, 3> operator*(const  RayType<ScalarType, 3>& ray_) const {
        Vector4<ScalarType> o{ray_.origin.x(), ray_.origin.y(), ray_.origin.z(), ScalarType{1.0}};
        o = transform_ * o;
        Vector3<ScalarType> d{ray_.direction.x(), ray_.direction.y(), ray_.direction.z()};
        d = transform_.template topLeftCorner<3,3>() * d;
	    return RayType<ScalarType, 3>{
            Point3f{o.x(), o.y(), o.z()},
            Vector3f{d.x(), d.y(), d.z()},
            ray_.min_t,
            ray_.max_t
	    };
    }

	Transform44Type<Scalar> inverse() const {
	    return Transform44Type<Scalar>(inverse_, transform_);
	}

	const Matrix44<Scalar>& matrix() const {
	    return transform_;
	}

    bool operator==(const Transform44Type<Scalar> &t) const {
        if (transform_ != t.transform_) {
            return false;
        }

        return true;
    }

    bool operator!=(const Transform44Type<Scalar> &t) const {
        return !operator==(t);
    }

private:
	Matrix transform_;
	Matrix inverse_;
};

template <typename ScalarType>
Transform44Type<ScalarType> operator*(const Transform44Type<ScalarType>& lhs, const Transform44Type<ScalarType>& rhs) {
    Transform44Type<ScalarType> result(lhs.matrix() * rhs.matrix());
    return result;
}

template <typename ScalarType>
Transform44Type<ScalarType> identity() {
    return Transform44Type<ScalarType>(Eigen::Matrix<ScalarType, 4, 4>::Identity());
}

template <typename ScalarType>
Transform44Type<ScalarType> translate(const ScalarType x, const ScalarType y, const ScalarType z) {
    Eigen::Transform<ScalarType,3,Eigen::Affine> translation(Eigen::Translation<ScalarType, 3>(x, y, z));
    return Transform44Type<ScalarType>(translation.matrix());
}
template <typename ScalarType>
Transform44Type<ScalarType> translate(const ScalarType x, const ScalarType y) {
    return translate(x, y, ScalarType{0.0});
}
template <typename ScalarType>
Transform44Type<ScalarType> translate(const VectorType<ScalarType, 2>& v) {
    return translate(v.x(), v.y(), ScalarType{0.0});
}
template <typename ScalarType>
Transform44Type<ScalarType> translate(const VectorType<ScalarType, 3>& v) {
    return translate(v.x(), v.y(), v.z());
}

template <typename ScalarType>
Transform44Type<ScalarType> scale(const ScalarType x, const ScalarType y, const ScalarType z) {
    Eigen::Transform<ScalarType,3,Eigen::Affine> transform(Eigen::Scaling(x, y, z));
    return Transform44Type<ScalarType>(transform.matrix());
}
template <typename ScalarType>
Transform44Type<ScalarType> scale(const ScalarType s) {
    return scale(s, s, s);
}
template <typename ScalarType>
Transform44Type<ScalarType> scale(const ScalarType x, const ScalarType y) {
    Eigen::Transform<ScalarType,3,Eigen::Affine> transform(Eigen::Scaling(x, y, ScalarType{1.0}));
    return Transform44Type<ScalarType>(transform.matrix());
}

template <typename ScalarType>
Transform44Type<ScalarType> rotate_x(const ScalarType radian) {
    ScalarType s = sin(radian);
    ScalarType c = cos(radian);

    Matrix44<ScalarType> m;
    m.row(0) = Eigen::Matrix<ScalarType, 4, 1>(1, 0, 0, 0);
    m.row(1) = Eigen::Matrix<ScalarType, 4, 1>(0, c, -s, 0);
    m.row(2) = Eigen::Matrix<ScalarType, 4, 1>(0, s, c, 0);
    m.row(3) = Eigen::Matrix<ScalarType, 4, 1>(0, 0, 0, 1);

    return Transform44Type<ScalarType>{m};
}

template <typename ScalarType>
Transform44Type<ScalarType> rotate_y(const ScalarType radian) {
    ScalarType s = sin(radian);
    ScalarType c = cos(radian);

    Matrix44<ScalarType> m;
    m.row(0) = Eigen::Matrix<ScalarType, 4, 1>(c, 0, s, 0);
    m.row(1) = Eigen::Matrix<ScalarType, 4, 1>(0, 1, 0, 0);
    m.row(2) = Eigen::Matrix<ScalarType, 4, 1>(-s, 0, c, 0);
    m.row(3) = Eigen::Matrix<ScalarType, 4, 1>(0, 0, 0, 1);

    return Transform44Type<ScalarType>{m};
}

template <typename ScalarType>
Transform44Type<ScalarType> rotate_z(const ScalarType radian) {
    ScalarType s = sin(radian);
    ScalarType c = cos(radian);

    Matrix44<ScalarType> m;
    m.row(0) = Eigen::Matrix<ScalarType, 4, 1>(c, -s, 0, 0);
    m.row(1) = Eigen::Matrix<ScalarType, 4, 1>(s, c, 0, 0);
    m.row(2) = Eigen::Matrix<ScalarType, 4, 1>(0, 0, 1, 0);
    m.row(3) = Eigen::Matrix<ScalarType, 4, 1>(0, 0, 0, 1);

    return Transform44Type<ScalarType>{m};
}

template <typename ScalarType>
Transform44Type<ScalarType> look_at(const Point2<ScalarType> & origin, const Point2<ScalarType>& target) {
    assert(origin != target);
    Vector2<ScalarType> forward = target - origin;
    forward.normalize();
    Vector2<ScalarType> up{-forward.y(), forward.x()};

    Matrix44<ScalarType> transform;
    transform << forward.x(), up.x(), ScalarType{0.0}, origin.x(),
                 forward.y(), up.y(), ScalarType{0.0}, origin.y(),
                 ScalarType{0.0}, ScalarType{0.0}, ScalarType{1.0}, ScalarType{0.0},
                 ScalarType{0.0}, ScalarType{0.0}, ScalarType{0.0}, ScalarType{1.0};

    return Transform44Type<ScalarType>{(transform).inverse()};
}

template <typename ScalarType>
Transform44Type<ScalarType> look_at(const Point3<ScalarType> &origin,
                                    const Point3<ScalarType> &target,
                                    const Vector3<ScalarType> &up) {
    assert(origin != target);
    Vector3<ScalarType> forward = target - origin;
    forward.normalize();

    if(up.normalized().cross(forward).norm() == 0) {
        throw std::runtime_error("invalid up vector");
    }

    Vector3f right = up.normalized().cross(forward).normalized();

    Vector3f corrected_up = forward.cross(right);

    Matrix44<ScalarType> transform;
    transform << right.x(), corrected_up.x(), forward.x(), origin.x(),
                 right.y(), corrected_up.y(), forward.y(), origin.y(),
                 right.z(), corrected_up.z(), forward.z(), origin.z(),
                 ScalarType{0.0}, ScalarType{0.0}, ScalarType{0.0}, ScalarType{1.0};

    return Transform44Type<ScalarType>{(transform).inverse()};
}

template <typename ScalarType>
std::ostream &operator<<(std::ostream &os, const Transform44Type<ScalarType> &transform) {
    os << "Transform44f" << "[" << std::endl
       << "  matrix = " << transform.matrix()
       << "]"
       << std::endl;
    return os;
}

template <typename ScalarType>
Transform44Type<ScalarType> perspective(
        const ScalarType vertical_fov,
        const ScalarType near_clip_plane_distance,
        const ScalarType far_clip_plane_distance) {

    Matrix44<ScalarType> P;
    P << 1, 0, 0, 0,
         0, 1, 0, 0,
         0, 0, far_clip_plane_distance / (far_clip_plane_distance - near_clip_plane_distance), -(far_clip_plane_distance * near_clip_plane_distance) / (far_clip_plane_distance - near_clip_plane_distance),
         0, 0, 1, 0;

    auto s = ScalarType{1}/tan(vertical_fov / ScalarType{2});

    Matrix44<ScalarType> S;
    S <<  s, 0, 0, 0,
          0, s, 0, 0,
          0, 0, 1, 0,
          0, 0, 0, 1;

    return Transform44Type<ScalarType>{S * P};
}

template <typename ScalarType>
Transform44Type<ScalarType> raster_space_to_ndc(const Vector2<ScalarType>& film_size_f) {
    auto raster_space_to_ndc = translate(-1.f, -1.f, 0.f) *
                               scale(2.f, 2.f, 1.f) *
                               translate(0.f, 1.f, 0.f) *
                               scale(1.f, -1.f, 1.f) *
                               scale(1.f/film_size_f.x(), 1.f/film_size_f.y(), 1.0f);

    return raster_space_to_ndc;
}

using Transform44f = Transform44Type<float>;
using Transform44d = Transform44Type<double>;

/*
template<>
const auto identity44f = identity<float>;

template<>
const auto identity44d = identity<float>;

template<>
const auto translate44f = translate<float>;

template<>
const auto translate44d = translate<double>;
*/

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Math_Transform_5c51bd58_f3a5_4ce3_9921_c4906c57beb2_h
