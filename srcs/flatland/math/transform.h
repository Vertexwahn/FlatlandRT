/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Transform2_5c51bd58_f3a5_4ce3_9921_c4906c57beb2_h
#define Flatland_Transform2_5c51bd58_f3a5_4ce3_9921_c4906c57beb2_h

#include "flatland/core/namespace.h"
#include "flatland/math/matrix.h"
#include "flatland/math/normal.h"
#include "flatland/math/point.h"
#include "flatland/math/ray.h"
#include "flatland/math/vector.h"

#include <iostream>

FLATLAND_BEGIN_NAMESPACE

template <typename ScalarType>
class Transform44Type {
public:
    using Matrix = Matrix44<ScalarType>;
    using Vector = VectorType<2, ScalarType>;
    using Point = PointType<2, ScalarType>;
    using Normal = NormalType<2, ScalarType>;
    using Scalar = ScalarType;
    using Ray = RayType<2, ScalarType>;

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

    Vector operator*(const Vector& p) const {
        Vector4<ScalarType> v{p.x(), p.y(), Scalar{0.0}, Scalar{0.0}};
        Vector4<ScalarType> vt = transform_ * v;
        return Point{vt.x(), vt.y()};
    }

	Point operator*(const Point& p) const {
        Point4<ScalarType> p4{p.x(), p.y(), Scalar{0.0}, Scalar{1.0}};
        Point4<ScalarType> pt = transform_ * p4;
		return Point{pt.x(), pt.y()};
	}

    Normal operator*(const Normal& normal) const {
        Vector4<ScalarType> v{normal.x(), normal.y(), Scalar{0.0}, Scalar{0.0}};
        auto vt = transform_.inverse().transpose() * v;
        return Normal{vt.x(), vt.y()};
    }

    Ray operator*(const Ray& ray_) const {
        Vector4<ScalarType> o{ray_.origin.x(), ray_.origin.y(), ScalarType{0.0}, ScalarType{1.0}};
        o = transform_ * o;
        Vector3<ScalarType> d{ray_.direction.x(), ray_.direction.y(), ScalarType{0.0}};
        d = transform_.template topLeftCorner<3,3>() * d;
	    return Ray{
            Point2f{o.x(), o.y()},
            Vector2f{d.x(), d.y()},
            ray_.min_t,
            ray_.max_t
	    };
    }

	Transform44Type<Scalar> inverse() const {
	    return Transform44Type<Scalar>(inverse_, transform_);
	}

	const Matrix44<Scalar>& getMatrix() const {
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
    Transform44Type<ScalarType> result(lhs.getMatrix() * rhs.getMatrix());
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
Transform44Type<ScalarType> translate(const Vector2<ScalarType>& v) {
    return translate(v.x(), v.y(), ScalarType{0.0});
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
Transform44Type<ScalarType> rotateZ(const ScalarType radian) {

    ScalarType s = sin(radian);
    ScalarType c = cos(radian);

    Matrix44<ScalarType> m;
    m.row(0) = Eigen::Matrix<ScalarType, 4, 1>(c, -s, 0, 0);
    m.row(1) = Eigen::Matrix<ScalarType, 4, 1>(s, c, 0, 0);
    m.row(2) = Eigen::Matrix<ScalarType, 4, 1>(0, 0, 1, 0);
    m.row(3) = Eigen::Matrix<ScalarType, 4, 1>(0, 0, 0, 1);

    return Transform44Type<ScalarType>{m};


/*
    Eigen::Transform<ScalarType,3,Eigen::Affine> transform(Eigen::AngleAxis<ScalarType>(radian,
                              Eigen::Matrix<ScalarType, 3, 1>(ScalarType{0.0}, ScalarType{1.0}, ScalarType{0.0})));
    return Transform44Type<ScalarType>(transform.matrix());
*/
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

    return Transform44Type<ScalarType>((transform).inverse());
}

template <typename ScalarType>
std::ostream &operator<<(std::ostream &os, const Transform44Type<ScalarType> &transform) {
    os << "Transform44f" << "[" << std::endl
       << "  matrix = " << transform.getMatrix()
       << "]"
       << std::endl;
    return os;
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

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Transform2_5c51bd58_f3a5_4ce3_9921_c4906c57beb2_h