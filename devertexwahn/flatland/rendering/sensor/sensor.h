/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_Sensor2_84975de0_f654_425b_85f1_5cbf7a1f77eb_h
#define De_Vertexwahn_Core_Sensor2_84975de0_f654_425b_85f1_5cbf7a1f77eb_h

#include "core/namespace.h"
#include "core/object.h"
#include "math/transform.h"
#include "flatland/rendering/property_set.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template <unsigned int Dimension, typename ScalarType>
class SensorTypeBase : public Object {
public:
    using Point = PointType<Dimension, ScalarType>;
    using Vector = VectorType<Dimension, ScalarType>;
    using Ray = RayType<Dimension, ScalarType>;
    using Scalar = ScalarType;
    using Transform = Transform44Type<ScalarType>;

    SensorTypeBase(const PropertySet& ps) {
    }

    ~SensorTypeBase() {}

    void set_transform(const Transform &transform) {
        world_to_sensor_ = transform;
    }
    const Transform &transform() const {
        return world_to_sensor_;
    }

    virtual Ray generate_ray(const Point2f& raster_position) const  = 0;

    std::string to_string() const override {
        return "SensorType";
    }

protected:
    Transform world_to_sensor_;
};

template <unsigned int Dimension, typename ScalarType>
class SensorType : public SensorTypeBase<Dimension, ScalarType> {};

class Film_ : public Object {
public:
    Film_(const PropertySet& ps) {
        const int width = ps.get_property<int>("width");
        const int height = ps.get_property<int>("height");
        size_ = Vector2i{width, height};
        filename_ = ps.get_property<std::string>("filename");
    }

    std::string to_string() const override {
        return "Film";
    }

    const Vector2i& size() const {
        return size_;
    }

    int width() const {
        return size_.x();
    }
    int height() const {
        return size_.y();
    }

    std::string_view filename() const {
        return filename_;
    }

private:
    Vector2i size_;
    std::string filename_;
};

template <typename ScalarType>
class SensorType<2, ScalarType> : public SensorTypeBase<2, ScalarType> {
public:
    using Point = PointType<2, ScalarType>;
    using Vector = VectorType<2, ScalarType>;
    using Ray = RayType<2, ScalarType>;
    using Scalar = ScalarType;
    using Transform = Transform44Type<ScalarType>;

    SensorType(const PropertySet& ps) : SensorTypeBase<2,ScalarType>(ps) {
        film_ = std::dynamic_pointer_cast<Film_>(ps.get_property<ReferenceCounted<Object>>("film"));
        assert(film_);
    }

    virtual ~SensorType() {}

    Ray generate_ray(const Point2f& raster_position) const override {
        auto origin = raster_position;
        auto direction = Vector{Scalar{1.0}, Scalar{0.0}};
        auto min_t = Scalar{0.001};
        auto max_t = Scalar{1000.0};
        return SensorType<2, ScalarType>::world_to_sensor_.inverse() * Ray{origin, direction, min_t, max_t};
    }

    ReferenceCounted<Film_> film() {
        return film_;
    }

protected:
    ReferenceCounted<Film_> film_;
};

using Sensor2f = SensorType<2, float>;
using Sensor2d = SensorType<2, double>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define Okapi_Sensor2_84975de0_f654_425b_85f1_5cbf7a1f77eb_h
