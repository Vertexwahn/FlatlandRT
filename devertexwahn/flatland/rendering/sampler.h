/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Flatland_Rendering_Sampler_e951b5eb_3019_4678_be3e_764686c28239_h
#define De_Vertexwahn_Flatland_Rendering_Sampler_e951b5eb_3019_4678_be3e_764686c28239_h

#include "core/object.h"
#include "math/point.h"
#include "flatland/rendering/property_set.h"

#include "pcg_random.hpp"

#include <random>

DE_VERTEXWAHN_BEGIN_NAMESPACE

template<typename ScalarType>
class SamplerType : public Object {
public:
    SamplerType(const PropertySet &ps) {
        sample_count_ = ps.get_property<int>("sample_count", 1);
    }

    virtual ~SamplerType() {}

    virtual ScalarType next_1d() = 0;

    virtual Point2 <ScalarType> next_2d() = 0;

    int sample_count() const {
        return sample_count_;
    };
    void set_sample_count(int sample_count) {
        sample_count_ = sample_count;
    }

    virtual std::unique_ptr<SamplerType> clone() = 0;

protected:
    int sample_count_ = 1;
};

template<typename ScalarType>
class PixelCenterSamplerType : public SamplerType<ScalarType> {
public:
    using Scalar = ScalarType;
    using Point = Point2<ScalarType>;
    using Base = SamplerType<ScalarType>;

    PixelCenterSamplerType(const PropertySet &ps) : SamplerType<ScalarType>(ps) {}

    virtual ~PixelCenterSamplerType() {}

    Scalar next_1d() override {
        return Scalar{0.5};
    }

    Point next_2d() override {
        return Point(Scalar{0.5}, Scalar{0.5});
    }

    std::unique_ptr<SamplerType<ScalarType>> clone() override {
        PropertySet ps;
        ps.add_property("sample_count", Base::sample_count_);
        return std::make_unique<PixelCenterSamplerType>(ps);
    }

    virtual std::string to_string() const override {
        return "PixelCenterSampler";
    }
};

template<typename ScalarType>
class IndependentSamplerType : public SamplerType<ScalarType> {
public:
    using Base = SamplerType<ScalarType>;

    IndependentSamplerType(const PropertySet &ps) : SamplerType<ScalarType>(ps) {
        deterministic_seed_ = ps.get_property<bool>("deterministic_seed", false);

        if (!deterministic_seed_) {
            rng_.seed(pcg_extras::seed_seq_from<std::random_device>());
        }
    }

    virtual ~IndependentSamplerType() {}

    ScalarType next_1d() override {
        return uniform_distribution_(rng_);
    }

    Point2 <ScalarType> next_2d() override {
        auto x = uniform_distribution_(rng_);
        auto y = uniform_distribution_(rng_);
        return Point2<ScalarType>(x, y);
    }

    virtual std::string to_string() const override {
        return "IndependentSampler";
    }

    std::unique_ptr<SamplerType<ScalarType>> clone() override {
        PropertySet ps;
        ps.add_property("sample_count", Base::sample_count_);
        ps.add_property("deterministic_seed", deterministic_seed_);
        return std::make_unique<IndependentSamplerType>(ps);
    }

private:
    bool deterministic_seed_ = false;
    pcg32 rng_;
    std::uniform_real_distribution<ScalarType> uniform_distribution_{0.0, 1.0};
};

using Sampler2f = SamplerType<float>;
using Sampler2d = SamplerType<double>;

using PixelCenterSampler = PixelCenterSamplerType<float>;
using IndependentSampler = IndependentSamplerType<float>;

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Flatland_Rendering_Sampler_e951b5eb_3019_4678_be3e_764686c28239_h
