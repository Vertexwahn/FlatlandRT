/*
 *  SPDX-FileCopyrightText: Copyright 2022-2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "core/reference_counted.hpp"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

class Dummy {
public:
    Dummy() {
        count++;
    }

    ~Dummy() {
        count--;
    }

    inline static int count = 0;
};

TEST(Memory, Memory_When_ReferenceCountedObjectDies_ExpectZeroCount) {
    {
        ReferenceCounted<Dummy> foo = make_reference_counted<Dummy>();
    }

    EXPECT_TRUE(Dummy::count == 0);
}

TEST(Memory, Memory_RefCount) {
    ReferenceCounted<Dummy> foo = make_reference_counted<Dummy>();
    ReferenceCounted<Dummy> bar = foo;

    EXPECT_TRUE(Dummy::count == 1);

    foo = nullptr;
    bar = nullptr;

    EXPECT_TRUE(Dummy::count == 0);
}
