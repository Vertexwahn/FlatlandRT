/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/core/sampler.h"

#include <array>
#include <hypothesis.h>
#include <gmock/gmock.h>

using namespace Flatland;

TEST(Sampler, GivenSampler_WhenGettingAUniformRandomNumber_ExpectCertainAverageValue) {
    Sampler2f sampler{false};

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
    Sampler2f sampler{false};

    bool notAlwaysEqual = false;

    constexpr int rounds = 100000;
    float lastRandomNumber = sampler.next1d();
    for(int i = 0; i < rounds-1; ++i) {
        auto random_number = sampler.next1d();
        if(lastRandomNumber != random_number) {
            notAlwaysEqual = true;
        }
    }

    EXPECT_TRUE(notAlwaysEqual);
}

TEST(Sampler, GivenSampler_WhenGettingAUniformPoint_ExpectCertainAverageValue) {
    Sampler2f sampler{false};

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

TEST(Smampler, GivenSample_WhenDrawingSamples_ExpectUniformDistribution) {
    Sampler2f sampler{false};
    
    const int bucketCount = 100;
    double observedFrequencies[bucketCount] = {0};
    const int sampleCount = 100000;

    for (size_t i = 0; i < sampleCount; ++i) {
        auto random_number = sampler.next1d();
        int bucketIndex = std::floor(random_number * bucketCount);
        observedFrequencies[bucketIndex] += 1;
    }

    std::vector<double> expectedFrequencies;
    expectedFrequencies.resize(bucketCount);
    std::fill(std::begin(expectedFrequencies), std::end(expectedFrequencies), sampleCount/bucketCount);
        
    std::pair<bool, std::string> result = hypothesis::chi2_test(bucketCount, observedFrequencies, &expectedFrequencies[0], sampleCount, 5, 0.05);

    EXPECT_THAT(result.second, testing::HasSubstr("Chi^2 statistic = 120.37 (d.o.f. = 99)"));
    EXPECT_THAT(result.second, testing::HasSubstr("Accepted the null hypothesis"));
}

TEST(Sampler, WhenRequestingDeterministicBehaviour_ExpectDeterminsticBehaviour) {
    Sampler2f sampler1{false};
    Sampler2f sampler2{false};

    constexpr int rounds = 1000;

    std::array<float, rounds> random_array1{0};
    std::array<float, rounds> random_array2{0};

    for(int i = 0; i < rounds; ++i) {
        random_array1[i] = sampler1.next1d();
        random_array2[i] = sampler2.next1d();
    }

    EXPECT_THAT(random_array1, random_array2);
}

TEST(Sampler, WhenRequestingNonDeterministicBehaviour_ExpectNonDeterminsticBehaviour) {
    Sampler2f sampler1;
    Sampler2f sampler2;

    constexpr int rounds = 1000;

    std::array<float, rounds> random_array1{0};
    std::array<float, rounds> random_array2{0};

    for(int i = 0; i < rounds; ++i) {
        random_array1[i] = sampler1.next1d();
        random_array2[i] = sampler2.next1d();
    }

    EXPECT_THAT(random_array1, testing::Not(random_array2));
}
