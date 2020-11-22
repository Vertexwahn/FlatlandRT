/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_AmbientOcclusion_2ea71cc7_ef11_4e85_bb62_4f81a780fd35_h
#define Flatland_AmbientOcclusion_2ea71cc7_ef11_4e85_bb62_4f81a780fd35_h

#include "flatland/integrator/integrator.h"
#include "flatland/math/warping.h"

FLATLAND_BEGIN_NAMESPACE

template<unsigned int Dimension, typename ScalarType>
class AmbientOcclusion : public IntegratorType<Dimension, ScalarType> {
public:
    using Base = IntegratorType<2, ScalarType>;
    using Color = ColorType<3, ScalarType>;
    using Scalar = ScalarType;
    using Vector = VectorType<2, ScalarType>;
    using Point = PointType<2, ScalarType>;
    using Ray = Ray2<ScalarType>;
    using MediumEvent = MediumEvent2<ScalarType>;

    AmbientOcclusion(const PropertySet& ps) : IntegratorType<2, ScalarType>(ps) {
        background_color_ = ps.getProperty<Color3f>("background_color", Color3f{Scalar{0.0}, Scalar{0.0}, Scalar{0.0}});
        bRandomSampling_ = ps.getProperty<bool>("random_sampling", false);
        sampleCount_ = ps.getProperty<int>("sample_count", 10);
    }
    virtual ~AmbientOcclusion() {
    }

    Color trace(
            const Scene2<ScalarType> *scene,
            Ray &ray,
            const int depth) const override {
        MediumEvent me;
        bool hit = scene->intersect(ray, me);

        if(!hit)
            return background_color_;

        ray.max_t = me.t;
        Base::canvas_->add(ray);
    
        for(int i = 0; i < sampleCount_; ++i) {
            auto a = static_cast<Scalar>(i+1);
            auto b = static_cast<Scalar>(sampleCount_ + 1);
            Vector d = uniformSampleHalfCircle(a/b);

            if(bRandomSampling_)
                d = uniformSampleHalfCircle(Base::sampler_->next1d());

            d = me.frame.toWorld(d);
            d.normalize();

            Ray visibilityRay(me.p + d * Scalar{0.01}, d, Scalar{0.0}, Scalar{100.0});
            Base::canvas_->add(visibilityRay);
        }

        Vector d = sampleHalfCircle(Point(Scalar{0.7}, Scalar{0.7}));
        d.normalize();

        Ray visibilityRay(me.p + d * Scalar{0.01}, d, Scalar{0.0}, Scalar{100.0});
        if (scene->intersect(visibilityRay, me)) {
            //Base::canvas_->add(visibilityRay);
            return background_color_;
        }
        else {
            //Base::canvas_->add(visibilityRay);
            return Color{Scalar{1.0f}, Scalar{1.0f}, Scalar{1.0f}};
        }
    }

private:
    Color background_color_{Scalar{0.0}, Scalar{0.0}, Scalar{0.0}};
    bool bRandomSampling_ = false;
    int sampleCount_ = 10;
};

using AmbientOcclusion2f = AmbientOcclusion<2, float>;
using AmbientOcclusion2d = AmbientOcclusion<2, double>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_AmbientOcclusion_2ea71cc7_ef11_4e85_bb62_4f81a780fd35_h
