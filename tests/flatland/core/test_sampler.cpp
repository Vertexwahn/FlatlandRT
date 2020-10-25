/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/core/sampler.h"

#include <gmock/gmock.h>

using namespace Flatland;

TEST(Sampler, GivenSampler_WhenGettingAUniformRandomNumber_ExpectCertainAverageValue) {
    Sampler2f sampler(42u);

    float avg = 0.0f;
    constexpr int rounds = 100000;
    for(int i = 0; i < rounds; ++i) {
        auto random_number = sampler.next1d();
        avg += random_number;
    }
    avg /= rounds;

    EXPECT_NEAR(avg, 0.5f, 0.1f);
}

TEST(Sampler, GivenSampler_WhenGettingAUniformRandomNumber_ExpectNotAlwaysEqual) {
    Sampler2f sampler(42u);

    bool notAlwaysEqual = false;

    float avg = 0.0f;
    constexpr int rounds = 100000;
    float lastRandomNumber = sampler.next1d();
    for(int i = 0; i < rounds-1; ++i) {
        auto random_number = sampler.next1d();
        if(lastRandomNumber != random_number) {
            notAlwaysEqual = true;
        }
    }
    avg /= rounds;

    EXPECT_TRUE(notAlwaysEqual);
}

TEST(Sampler, GivenSampler_WhenGettingAUniformPoint_ExpectCertainAverageValue) {
    Sampler2f sampler(42u);

    Point2f avg{0.0f, 0.0f};
    constexpr int rounds = 100000;
    for(int i = 0; i < rounds; ++i) {
        auto random_number = sampler.next2d();
        avg.x() += random_number.x();
        avg.y() += random_number.y();
    }
    avg /= rounds;

    EXPECT_NEAR(avg.x(), 0.5f, 0.1f);
    EXPECT_NEAR(avg.y(), 0.5f, 0.1f);
}