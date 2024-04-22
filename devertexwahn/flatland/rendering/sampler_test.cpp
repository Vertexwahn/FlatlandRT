/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/sampler.h"

#include "hypothesis.h"

#include "gmock/gmock.h"

#include <array>

using namespace de_vertexwahn;

TEST(IndependentSampler, GivenSampler_WhenGettingAUniformRandomNumber_ExpectCertainAverageValue) {
    PropertySet ps;
    ps.add_property("deterministic_seed", true);
    IndependentSampler sampler{ps};

    float avg = 0.f;
    constexpr int rounds = 100000;
    for (int i = 0; i < rounds; ++i) {
        auto random_number = sampler.next_1d();
        avg += random_number;
    }
    avg /= rounds;

    EXPECT_NEAR(avg, 0.5f, 0.1f);
}

TEST(IndependentSampler, GivenSampler_WhenGettingAUniformRandomNumber_ExpectNotAlwaysEqual) {
    PropertySet ps;
    ps.add_property("deterministic_seed", true);
    IndependentSampler sampler{ps};

    bool not_always_equal = false;

    constexpr int rounds = 100000;
    float lastRandomNumber = sampler.next_1d();
    for (int i = 0; i < rounds - 1; ++i) {
        auto random_number = sampler.next_1d();
        if (lastRandomNumber != random_number) {
            not_always_equal = true;
        }
    }

    EXPECT_TRUE(not_always_equal);
}

TEST(IndependentSampler, GivenSampler_WhenGettingAUniformPoint_ExpectCertainAverageValue) {
    PropertySet ps;
    ps.add_property("deterministic_seed", true);
    IndependentSampler sampler{ps};

    Point2f avg{0.f, 0.f};
    constexpr int rounds = 100000;
    for (int i = 0; i < rounds; ++i) {
        auto random_number = sampler.next_2d();
        avg.x() += random_number.x();
        avg.y() += random_number.y();
    }
    avg /= rounds;

    EXPECT_NEAR(avg.x(), 0.5f, 0.1f);
    EXPECT_NEAR(avg.y(), 0.5f, 0.1f);
}

TEST(IndependentSampler, GivenSample_WhenDrawingSamples_ExpectUniformDistribution) {
    PropertySet ps;
    ps.add_property("deterministic_seed", true);
    IndependentSampler sampler{ps};

    const int bucket_count = 100;
    double observed_frequencies[bucket_count] = {0};
    const int sample_count = 100000;

    for (size_t i = 0; i < sample_count; ++i) {
        auto random_number = sampler.next_1d();
        int bucket_index = std::floor(random_number * bucket_count);
        observed_frequencies[bucket_index] += 1;
    }

    std::vector<double> expected_frequencies;
    expected_frequencies.resize(bucket_count);
    std::fill(std::begin(expected_frequencies), std::end(expected_frequencies), sample_count / bucket_count);

    std::pair<bool, std::string> result = hypothesis::chi2_test(
            bucket_count,
            observed_frequencies,
            &expected_frequencies[0],
            sample_count,
            5,
            0.05);

    EXPECT_THAT(result.second, testing::HasSubstr("Chi^2 statistic = 120.37 (d.o.f. = 99)"));
    EXPECT_THAT(result.second, testing::HasSubstr("Accepted the null hypothesis"));
}

TEST(IndependentSampler, WhenRequestingDeterministicBehaviour_ExpectDeterminsticBehaviour) {
    PropertySet ps;
    ps.add_property("deterministic_seed", true);
    IndependentSampler sampler1{ps};
    IndependentSampler sampler2{ps};

    constexpr int rounds = 1000;

    std::array<float, rounds> random_array1{0};
    std::array<float, rounds> random_array2{0};

    for (int i = 0; i < rounds; ++i) {
        random_array1[i] = sampler1.next_1d();
        random_array2[i] = sampler2.next_1d();
    }

    EXPECT_THAT(random_array1, random_array2);
}

TEST(IndependentSampler, WhenRequestingNonDeterministicBehaviour_ExpectNonDeterminsticBehaviour) {
    PropertySet ps;
    IndependentSampler sampler1{ps};
    IndependentSampler sampler2{ps};

    constexpr int rounds = 1000;

    std::array<float, rounds> random_array1{0};
    std::array<float, rounds> random_array2{0};

    for (int i = 0; i < rounds; ++i) {
        random_array1[i] = sampler1.next_1d();
        random_array2[i] = sampler2.next_1d();
    }

    EXPECT_THAT(random_array1, testing::Not(random_array2));
}

TEST(IndependentSampler, to_string) {
    PropertySet ps;
    IndependentSampler sampler{ps};

    EXPECT_THAT(sampler.to_string(), ::testing::HasSubstr("IndependentSampler"));
}

TEST(IndependentSampler, clone) {
    PropertySet ps;
    IndependentSampler sampler{ps};

    auto cloned_sampler = sampler.clone();

    EXPECT_THAT(cloned_sampler->to_string(), ::testing::HasSubstr("IndependentSampler"));
}

TEST(PixelCenterSampler, GivenPixelCenterSample_WhenRequestingSample_ThenExpectAlwaysCenterPosition) {
    ConstantSampler pcs{{}};

    EXPECT_THAT(pcs.next_1d(), .5f);
    EXPECT_THAT(pcs.next_2d(), Point2f(.5f, .5f));
}
