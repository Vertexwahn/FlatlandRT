/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#include "flatland/core/reference_counted.h"

#include <gmock/gmock.h>

using namespace Flatland;

class Dummy {
public:
    Dummy() {
        count++;
    }
    ~Dummy() {
        count--;
    }

    static int count;
};

int Dummy::count = 0;

TEST(Memory, Memory_When_RefrenceCountedObjectDies_ExpectoZeroCount) {
    {
        ReferenceCounted<Dummy> foo = makeReferenceCounted<Dummy>();
    }

    EXPECT_TRUE(Dummy::count == 0);
}

TEST(Memory, Memory_RefCount) {
    ReferenceCounted<Dummy> foo = makeReferenceCounted<Dummy>();
    ReferenceCounted<Dummy> bar = foo;

    EXPECT_TRUE(Dummy::count == 1);
}

