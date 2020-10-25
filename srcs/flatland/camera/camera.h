/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Camera_14b27f8d_8dca_49d6_8087_3d67c31d29ec_h
#define Flatland_Camera_14b27f8d_8dca_49d6_8087_3d67c31d29ec_h

#include "srcs/flatland/camera/film.h"
#include "srcs/flatland/math/transform.h"
#include "srcs/flatland/core/namespace.h"

#include <iostream>
#include <string>

FLATLAND_BEGIN_NAMESPACE

template <unsigned int Dimension, typename ScalarType>
class CameraType {
public:
    using Point = PointType<Dimension, ScalarType>;
    using Vector = VectorType<Dimension, ScalarType>;
    using Ray = RayType<Dimension, ScalarType>;
    using Scalar = ScalarType;
    using Transform = Transform44Type<ScalarType>;

    CameraType(const int width,
               const int height,
               std::string_view filename) :
            film_(width, height, filename) {}

    virtual ~CameraType() {}

    const Film& getFilm() const {
        return film_;
    }

    void setTransform(const Transform &transform) {
        worldToCamera_ = transform;
    }
    const Transform &getTransform() const {
        return worldToCamera_;
    }

    Ray generateRay() const {
        auto origin = Point{Scalar{0.0}, Scalar{0.0}};
        auto direction = Vector{Scalar{1.0}, Scalar{0.0}};
        auto min_t = Scalar{0.001};
        auto max_t = Scalar{1000.0};
        return worldToCamera_.inverse() * Ray{origin, direction, min_t, max_t};
    }

private:
    Film film_;
    Transform worldToCamera_;
};

using Camera2f = CameraType<2, float>;
using Camera2d = CameraType<2, double>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Camera_14b27f8d_8dca_49d6_8087_3d67c31d29ec_h
