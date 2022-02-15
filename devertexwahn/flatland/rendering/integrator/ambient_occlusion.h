/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Core_AmbientOcclusion2_b609d24d_c389_4692_82df_bcb7e77f756a_h
#define Flatland_Core_AmbientOcclusion2_b609d24d_c389_4692_82df_bcb7e77f756a_h

#include "flatland/core/namespace.h"
#include "flatland/math/warping.h"
#include "flatland/rendering/integrator/integrator.h"

FLATLAND_BEGIN_NAMESPACE

template<unsigned int Dimension, typename ScalarType>
class AmbientOcclusion : public IntegratorType<Dimension, ScalarType> {
public:
    using Base = IntegratorType<Dimension, ScalarType>;
    using Color = ColorType<3, ScalarType>;
    using Scalar = ScalarType;
    using Vector = VectorType<Dimension, ScalarType>;
    using Point = PointType<Dimension, ScalarType>;
    using Ray = RayType<Dimension, ScalarType>;
    using MediumEvent = MediumEventType<Dimension, ScalarType>;
    using Sampler = SamplerType<ScalarType>;

    AmbientOcclusion(const PropertySet& ps) : IntegratorType<Dimension, ScalarType>(ps) {
        background_color_ = ps.get_property<Color3f>("background_color", Color3f{Scalar{0.0}, Scalar{0.0}, Scalar{0.0}});
        use_random_sampling_ = ps.get_property<bool>("random_sampling", false);
        sample_count_ = ps.get_property<int>("sample_count", 10);
    }
    virtual ~AmbientOcclusion() {
    }

    Color trace(
            const SceneType<Dimension, ScalarType> *scene,
            Sampler *sampler,
            Ray &ray,
            const int depth) const override {
        MediumEvent me;
        bool hit = scene->intersect(ray, me);

        if(!hit)
            return background_color_;

        ray.max_t = me.t;
        Base::canvas_->add(ray);

        for(int i = 0; i < sample_count_; ++i) {
            auto a = static_cast<Scalar>(i+1);
            auto b = static_cast<Scalar>(sample_count_ + 1);
            Vector d = uniform_sample_half_circle(a / b);

            if(use_random_sampling_)
                d = uniform_sample_half_circle(sampler->next_1d());

            d = me.geo_frame.to_world(d);
            d.normalize();

            Ray visibility_ray(me.p + d * Scalar{0.01}, d, Scalar{0.0}, Scalar{100.0});
            Base::canvas_->add(visibility_ray);
        }

        Vector d = sample_half_circle(Point(Scalar{0.7}, Scalar{0.7}));
        d.normalize();

        Ray visibility_ray(me.p + d * Scalar{0.01}, d, Scalar{0.0}, Scalar{100.0});

        if (scene->intersect(visibility_ray, me)) {
            return background_color_;
        }
        else {
            return Color{Scalar{1.f}, Scalar{1.f}, Scalar{1.f}};
        }
    }

private:
    Color background_color_{Scalar{0.0}, Scalar{0.0}, Scalar{0.0}};
    bool use_random_sampling_ = false;
    int sample_count_ = 10;
};

using AmbientOcclusion2f = AmbientOcclusion<2, float>;
using AmbientOcclusion2d = AmbientOcclusion<2, double>;

FLATLAND_END_NAMESPACE

#endif // end define Okapi_AmbientOcclusion2_b609d24d_c389_4692_82df_bcb7e77f756a_h

