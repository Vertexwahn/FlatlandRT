/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Sampler_e951b5eb_3019_4678_be3e_764686c28239_h
#define Flatland_Sampler_e951b5eb_3019_4678_be3e_764686c28239_h

#include "flatland/core/namespace.h"
#include "flatland/math/point.h"

#include <pcg_random.hpp>

#include <random>

FLATLAND_BEGIN_NAMESPACE

template <typename ScalarType>
class SamplerType {
public:
    SamplerType(bool non_determistic_seed = true) {
        if(non_determistic_seed)
            rng.seed(pcg_extras::seed_seq_from<std::random_device>());
    }

    virtual ~SamplerType() {
    }

    ScalarType next1d() {
        return uniform_distribution(rng);
    }

    Point2<ScalarType> next2d() {
        auto x = uniform_distribution(rng);
        auto y = uniform_distribution(rng);
        return Point2<ScalarType>(x, y);
    }

private:
    pcg32 rng;
    std::uniform_real_distribution<ScalarType> uniform_distribution {0.0, 1.0};
};

using Sampler2f = SamplerType<float>;
using Sampler2d = SamplerType<double>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Sampler_e951b5eb_3019_4678_be3e_764686c28239_h
