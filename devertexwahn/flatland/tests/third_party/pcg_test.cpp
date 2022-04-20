/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "pcg_random.hpp"

#include <random>

#include "gmock/gmock.h"

TEST(pcg, generte_random_number) {
    //pcg_extras::seed_seq_from<std::random_device> seed_source;
    //pcg32 rng(seed_source);
    pcg32 rng(42u);

    std::uniform_real_distribution<float> uniform_distribution(0.f, 1.f);

    float avg = 0.f;
    constexpr int rounds = 100000;
    for(int i = 0; i < rounds; ++i) {
         auto random_number = uniform_distribution(rng);
         avg += random_number;
    }
    avg /= rounds;

    EXPECT_NEAR(avg, 0.5f, 0.1f);
}
