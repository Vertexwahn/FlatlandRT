/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "math/axis_aligned_bounding_box.hpp"
#include "math/ray.hpp"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

TEST(AxisAlignedBoundingBox2i, ctor_and_valid) {
    AxisAlignedBoundingBox2i a{{0,   0},
                               {100, 100}};

    EXPECT_THAT(a.min_.x(), 0);
    EXPECT_THAT(a.min_.y(), 0);
    EXPECT_THAT(a.max_.y(), 100);
    EXPECT_THAT(a.max_.y(), 100);
    EXPECT_TRUE(a.valid());
}

TEST(AxisAlignedBoundingBox2f, ctor_and_valid) {
    AxisAlignedBoundingBox2f a{{0.f,   0.f},
                               {100.f, 100.f}};

    EXPECT_THAT(a.min_.x(), 0.f);
    EXPECT_THAT(a.min_.y(), 0.f);
    EXPECT_THAT(a.max_.y(), 100.f);
    EXPECT_THAT(a.max_.y(), 100.f);
    EXPECT_TRUE(a.valid());
}

TEST(AxisAlignedBoundingBox2i, GivenInvalidAABB_WhenValidation_ExpectNotValid) {
    Point2i min_v{0, 0};
    Point2i wrong_max_v{-100, 100};

    AxisAlignedBoundingBox2i bounds{min_v, wrong_max_v};

    EXPECT_FALSE(bounds.valid());
}

TEST(AxisAlignedBoundingBox2i, GivenAABB_WhenComputingWidhtAndHeight_ExpectCorrectResults) {
    Point2i min_v{0, 0};
    Point2i max_v{100, 100};

    AxisAlignedBoundingBox2i bounds{min_v, max_v};

    EXPECT_THAT(bounds.width(), 100);
    EXPECT_THAT(bounds.height(), 100);
}

TEST(AxisAlignedBoundingBox2f, GivenAABB_WhenComputingWidhtAndHeight_ExpectCorrectResults) {
    Point2f min_v{0.f, 0.f};
    Point2f max_v{100.f, 100.f};

    AxisAlignedBoundingBox2f bounds{min_v, max_v};

    EXPECT_THAT(bounds.width(), 100.f);
    EXPECT_THAT(bounds.height(), 100.f);
}

TEST(AxisAlignedBoundingBox2i, GivenSameBox_WhenCompare_ExpectCorrectResult) {
    Point2i min_v{0, 0};
    Point2i max_v{100, 100};

    AxisAlignedBoundingBox2i a{min_v, max_v};
    AxisAlignedBoundingBox2i b{min_v, max_v};

    EXPECT_THAT(a, b);
}

TEST(AxisAlignedBoundingBox3i, compare_equal) {
    Point3i min_v{0, 0, 0};
    Point3i max_v{100, 100, 100};

    AxisAlignedBoundingBox3i a{min_v, max_v};
    AxisAlignedBoundingBox3i b{min_v, max_v};

    EXPECT_THAT(a, b);
}

TEST(AxisAlignedBoundingBox3i, compare_not_equal) {
    AxisAlignedBoundingBox3i a{{0,   0,   0},
                               {100, 100, 100}};
    AxisAlignedBoundingBox3i b{{1,   0,   0},
                               {100, 100, 100}};

    EXPECT_FALSE(a == b);
}

TEST(AxisAlignedBoundingBox2i, size) {
    Point2i min_v{0, 0};
    Point2i max_v{100, 100};
    AxisAlignedBoundingBox2i bounds{min_v, max_v};

    EXPECT_THAT(bounds.size(), (Vector2i{100, 100}));
}

// test function "overlaps"
class AxisAlignedBoundingBox2iOverlapTests
        : public ::testing::TestWithParam<std::tuple<AxisAlignedBoundingBox2i, AxisAlignedBoundingBox2i, bool>> {
protected:
};

TEST_P(AxisAlignedBoundingBox2iOverlapTests, overlap) {
    AxisAlignedBoundingBox2i a = std::get<0>(GetParam());
    AxisAlignedBoundingBox2i b = std::get<1>(GetParam());

    bool does_overlap = overlaps(a, b);

    ASSERT_THAT(a.valid(), true);
    ASSERT_THAT(b.valid(), true);
    EXPECT_THAT(does_overlap, std::get<2>(GetParam()));
}

/*
 *  Case A:
 *           (50|150)  (150|150)
 *              x----------x
 *       (0|100)| (100|100)|
 *         x----+----x     |
 *         |    |    |     |
 *         |    x----+-----x
 *         | (50|50) | (150|50)
 *         x---------x
 *       (0|0)   (100|0)
 */

INSTANTIATE_TEST_SUITE_P(
        CheckOvelapp,
        AxisAlignedBoundingBox2iOverlapTests,
        ::testing::Values(
                std::make_tuple(AxisAlignedBoundingBox2i{{0,   0},
                                                         {100, 100}}, AxisAlignedBoundingBox2i{{0,   0},
                                                                                               {100, 100}},
                                true), // a and b identical
                std::make_tuple(AxisAlignedBoundingBox2i{{25, 25},
                                                         {75, 75}}, AxisAlignedBoundingBox2i{{0,   0},
                                                                                             {100, 100}},
                                true), // a inside b
                std::make_tuple(AxisAlignedBoundingBox2i{{0,   0},
                                                         {100, 100}}, AxisAlignedBoundingBox2i{{25, 25},
                                                                                               {75, 75}},
                                true), // b inside a
                std::make_tuple(AxisAlignedBoundingBox2i{{0,   0},
                                                         {100, 100}}, AxisAlignedBoundingBox2i{{0 + 50,   0},
                                                                                               {100 + 50, 100}},
                                true), // b overlaps right side of a
                std::make_tuple(AxisAlignedBoundingBox2i{{0,   0},
                                                         {100, 100}}, AxisAlignedBoundingBox2i{{0 - 50,   0},
                                                                                               {100 - 50, 100}},
                                true), // b overlaps left side of a
                std::make_tuple(AxisAlignedBoundingBox2i{{0,   0},
                                                         {100, 100}}, AxisAlignedBoundingBox2i{{0,   0 + 50},
                                                                                               {100, 100 + 50}},
                                true), // b overlaps top side of a
                std::make_tuple(AxisAlignedBoundingBox2i{{0,   0},
                                                         {100, 100}}, AxisAlignedBoundingBox2i{{0,   0 - 50},
                                                                                               {100, 100 - 50}},
                                true), // b overlaps bottom side of a
                std::make_tuple(AxisAlignedBoundingBox2i{{0,   0},
                                                         {100, 100}}, AxisAlignedBoundingBox2i{{0 + 50,   0 + 50},
                                                                                               {100 + 50, 100 + 50}},
                                true), // Case A
                std::make_tuple(AxisAlignedBoundingBox2i{{0,   0},
                                                         {100, 100}}, AxisAlignedBoundingBox2i{{50, 50},
                                                                                               {75, 75}}, true),
                std::make_tuple(AxisAlignedBoundingBox2i{{0,   0},
                                                         {100, 100}}, AxisAlignedBoundingBox2i{{300, 300},
                                                                                               {350, 350}}, false),
                std::make_tuple(AxisAlignedBoundingBox2i{{0,   0},
                                                         {100, 100}}, AxisAlignedBoundingBox2i{{-300, -300},
                                                                                               {-250, -250}}, false),
                std::make_tuple(AxisAlignedBoundingBox2i{{0,   0},
                                                         {100, 100}}, AxisAlignedBoundingBox2i{{100 + 0,   0},
                                                                                               {100 + 100, 100}},
                                true), // b touches a right
                std::make_tuple(AxisAlignedBoundingBox2i{{0,   0},
                                                         {100, 100}}, AxisAlignedBoundingBox2i{{0 - 100,   0},
                                                                                               {100 - 100, 100}},
                                true), // b touches a left
                std::make_tuple(AxisAlignedBoundingBox2i{{0,   0},
                                                         {100, 100}}, AxisAlignedBoundingBox2i{{0,   0 + 100},
                                                                                               {100, 100 + 100}},
                                true), // b touches a on top
                std::make_tuple(AxisAlignedBoundingBox2i{{0,   0},
                                                         {100, 100}}, AxisAlignedBoundingBox2i{{0,   0 - 100},
                                                                                               {100, 100 - 100}},
                                true), // b touches a on bottom
                std::make_tuple(AxisAlignedBoundingBox2i{{0,   0},
                                                         {100, 100}}, AxisAlignedBoundingBox2i{{100, 100},
                                                                                               {200, 200}},
                                true) // a and b are touching in one corner
        )
);

class AxisAlignedBoundingBox3iOverlapTests
        : public ::testing::TestWithParam<std::tuple<AxisAlignedBoundingBox3i, AxisAlignedBoundingBox3i, bool>> {
protected:
};

TEST_P(AxisAlignedBoundingBox3iOverlapTests, overlap) {
    AxisAlignedBoundingBox3i a = std::get<0>(GetParam());
    AxisAlignedBoundingBox3i b = std::get<1>(GetParam());

    bool does_overlap = overlaps(a, b);

    ASSERT_THAT(a.valid(), true);
    ASSERT_THAT(b.valid(), true);
    EXPECT_THAT(does_overlap, std::get<2>(GetParam()));
}

INSTANTIATE_TEST_SUITE_P(
        CheckOvelapp,
        AxisAlignedBoundingBox3iOverlapTests,
        ::testing::Values(
                std::make_tuple(AxisAlignedBoundingBox3i{{0,   0,   0},
                                                         {100, 100, 100}}, AxisAlignedBoundingBox3i{{0,   0,   0},
                                                                                                    {100, 100, 100}},
                                true), // a and b identical
                std::make_tuple(AxisAlignedBoundingBox3i{{25, 25, 25},
                                                         {75, 75, 75}}, AxisAlignedBoundingBox3i{{0,   0,   0},
                                                                                                 {100, 100, 100}},
                                true), // a inside b
                std::make_tuple(AxisAlignedBoundingBox3i{{0,   0,   0},
                                                         {100, 100, 100}}, AxisAlignedBoundingBox3i{{25, 25, 25},
                                                                                                    {75, 75, 75}},
                                true), // b inside a
                std::make_tuple(AxisAlignedBoundingBox3i{{0,   0,   0},
                                                         {100, 100, 100}}, AxisAlignedBoundingBox3i{{0 + 50,   0,   0},
                                                                                                    {100 +
                                                                                                     50,       100, 100}},
                                true), // b overlaps right side of a
                std::make_tuple(AxisAlignedBoundingBox3i{{0,   0,   0},
                                                         {100, 100, 100}}, AxisAlignedBoundingBox3i{{0 - 50,   0,   0},
                                                                                                    {100 -
                                                                                                     50,       100, 100}},
                                true), // b overlaps left side of a
                std::make_tuple(AxisAlignedBoundingBox3i{{0,   0,   0},
                                                         {100, 100, 100}}, AxisAlignedBoundingBox3i{{0,   0 + 50,   0},
                                                                                                    {100, 100 +
                                                                                                          50,       100}},
                                true), // b overlaps top side of a
                std::make_tuple(AxisAlignedBoundingBox3i{{0,   0,   0},
                                                         {100, 100, 100}}, AxisAlignedBoundingBox3i{{0,   0 - 50,   0},
                                                                                                    {100, 100 -
                                                                                                          50,       100}},
                                true), // b overlaps bottom side of a
                std::make_tuple(AxisAlignedBoundingBox3i{{0,   0,   0},
                                                         {100, 100, 100}},
                                AxisAlignedBoundingBox3i{{0 + 50,   0 + 50,   0 + 50},
                                                         {100 + 50, 100 + 50, 100 + 50}}, true), // Case A
                std::make_tuple(AxisAlignedBoundingBox3i{{0,   0,   0},
                                                         {100, 100, 100}}, AxisAlignedBoundingBox3i{{50, 50, 50},
                                                                                                    {75, 75, 75}},
                                true),
                std::make_tuple(AxisAlignedBoundingBox3i{{0,   0,   0},
                                                         {100, 100, 100}}, AxisAlignedBoundingBox3i{{300, 300, 300},
                                                                                                    {350, 350, 350}},
                                false),
                std::make_tuple(AxisAlignedBoundingBox3i{{0,   0,   0},
                                                         {100, 100, 100}}, AxisAlignedBoundingBox3i{{-300, -300, -300},
                                                                                                    {-250, -250, -250}},
                                false),
                std::make_tuple(AxisAlignedBoundingBox3i{{0,   0,   0},
                                                         {100, 100, 100}}, AxisAlignedBoundingBox3i{{100 + 0,   0,   0},
                                                                                                    {100 +
                                                                                                     100,       100, 100}},
                                true), // b touches a right
                std::make_tuple(AxisAlignedBoundingBox3i{{0,   0,   0},
                                                         {100, 100, 100}}, AxisAlignedBoundingBox3i{{0 - 100,   0,   0},
                                                                                                    {100 -
                                                                                                     100,       100, 100}},
                                true), // b touches a left
                std::make_tuple(AxisAlignedBoundingBox3i{{0,   0,   0},
                                                         {100, 100, 100}}, AxisAlignedBoundingBox3i{{0,   0 + 100,   0},
                                                                                                    {100, 100 +
                                                                                                          100,       100}},
                                true), // b touches a on top
                std::make_tuple(AxisAlignedBoundingBox3i{{0,   0,   0},
                                                         {100, 100, 100}}, AxisAlignedBoundingBox3i{{0,   0 - 100,   0},
                                                                                                    {100, 100 -
                                                                                                          100,       100}},
                                true), // b touches a on bottom
                std::make_tuple(AxisAlignedBoundingBox3i{{0,   0,   0},
                                                         {100, 100, 100}}, AxisAlignedBoundingBox3i{{100, 100, 100},
                                                                                                    {200, 200, 200}},
                                true) // a and b are touching in one corner
        )
);

TEST(AxisAlignedBoundingBox2i, intersect) {
    // Arrange
    Point2f min_v{100.f, 0.f};
    Point2f max_v{200.f, 100.f};
    AxisAlignedBoundingBox2f bounds{min_v, max_v};

    Ray2f ray{{0.f, 50.f}, {1.f, 0.f}, 0.f, 1000.f};

    // Act
    float hit{0.f};
    bool result = bounds.intersect(ray.origin, ray.direction, ray.max_t, &hit);

    // Assert
    EXPECT_THAT(result, true);
    EXPECT_THAT(hit, 100.f);
    //EXPECT_THAT(hit1, 200.f);
}

// test function "union_aabb"
class AxisAlignedBoundingBox2iUnionTests
        : public ::testing::TestWithParam<std::tuple<AxisAlignedBoundingBox2i, AxisAlignedBoundingBox2i, AxisAlignedBoundingBox2i>> {
protected:
};

TEST_P(AxisAlignedBoundingBox2iUnionTests, union_aabb) {
    AxisAlignedBoundingBox2i a = std::get<0>(GetParam());
    AxisAlignedBoundingBox2i b = std::get<1>(GetParam());

    auto c = union_aabb(a, b);
    auto d = union_aabb(b, a);

    ASSERT_THAT(a.valid(), true);
    ASSERT_THAT(b.valid(), true);
    ASSERT_THAT(c.valid(), true);
    ASSERT_THAT(d.valid(), true);

    EXPECT_THAT(c, std::get<2>(GetParam()));
    EXPECT_THAT(d, std::get<2>(GetParam()));
}


/*
 *   a and b next to each other
 *
 *            (1|1)     (2|1)
 *    x---------x---------x
 *    |         |         |
 *    |   (A)   |   (B)   |
 *    |         |         |
 *    x---------x---------x
 *  (0|0)     (1|0)
 */


INSTANTIATE_TEST_SUITE_P(
        CheckOvelapp,
        AxisAlignedBoundingBox2iUnionTests,
        ::testing::Values(
                std::make_tuple(AxisAlignedBoundingBox2i{{0,   0},
                                                         {100, 100}}, AxisAlignedBoundingBox2i{{0,   0},
                                                                                               {100, 100}},
                                AxisAlignedBoundingBox2i{{0,   0},
                                                         {100, 100}}), // a and b identical
                std::make_tuple(AxisAlignedBoundingBox2i{{25, 25},
                                                         {75, 75}}, AxisAlignedBoundingBox2i{{0,   0},
                                                                                             {100, 100}},
                                AxisAlignedBoundingBox2i{{0,   0},
                                                         {100, 100}}), // a inside b
                std::make_tuple(AxisAlignedBoundingBox2i{{0, 0},
                                                         {1, 1}}, AxisAlignedBoundingBox2i{{1, 0},
                                                                                           {2, 1}},
                                AxisAlignedBoundingBox2i{{0, 0},
                                                         {2, 1}}), // a and b next to each other
                std::make_tuple(AxisAlignedBoundingBox2i{{0, 0},
                                                         {1, 1}}, AxisAlignedBoundingBox2i{{2, 0},
                                                                                           {3, 1}},
                                AxisAlignedBoundingBox2i{{0, 0},
                                                         {3, 1}}),
                std::make_tuple(AxisAlignedBoundingBox2i{{0, 0},
                                                         {1, 1}}, AxisAlignedBoundingBox2i{{0, 2},
                                                                                           {1, 3}},
                                AxisAlignedBoundingBox2i{{0, 0},
                                                         {1, 3}}),
                std::make_tuple(AxisAlignedBoundingBox2i{{0, 0},
                                                         {1, 1}}, AxisAlignedBoundingBox2i{{2, 2},
                                                                                           {3, 3}},
                                AxisAlignedBoundingBox2i{{0, 0},
                                                         {3, 3}}),
                std::make_tuple(AxisAlignedBoundingBox2i{{-1, -1},
                                                         {0,  0}}, AxisAlignedBoundingBox2i{{2, 2},
                                                                                            {3, 3}},
                                AxisAlignedBoundingBox2i{{-1, -1},
                                                         {3,  3}})
        )
);

class AxisAlignedBoundingBox3iUnionTests
        : public ::testing::TestWithParam<std::tuple<AxisAlignedBoundingBox3i, AxisAlignedBoundingBox3i, AxisAlignedBoundingBox3i>> {
protected:
};

TEST_P(AxisAlignedBoundingBox3iUnionTests, union_aabb) {
    AxisAlignedBoundingBox3i a = std::get<0>(GetParam());
    AxisAlignedBoundingBox3i b = std::get<1>(GetParam());

    auto c = union_aabb(a, b);
    auto d = union_aabb(b, a);

    ASSERT_THAT(a.valid(), true);
    ASSERT_THAT(b.valid(), true);
    ASSERT_THAT(c.valid(), true);
    ASSERT_THAT(d.valid(), true);

    EXPECT_THAT(c, std::get<2>(GetParam()));
    EXPECT_THAT(d, std::get<2>(GetParam()));
}

INSTANTIATE_TEST_SUITE_P(
        CheckOvelapp,
        AxisAlignedBoundingBox3iUnionTests,
        ::testing::Values(
                std::make_tuple(AxisAlignedBoundingBox3i{{0,   0,   0},
                                                         {100, 100, 100}}, AxisAlignedBoundingBox3i{{0,   0,   0},
                                                                                                    {100, 100, 100}},
                                AxisAlignedBoundingBox3i{{0,   0,   0},
                                                         {100, 100, 100}}), // a and b identical
                std::make_tuple(AxisAlignedBoundingBox3i{{25, 25, 25},
                                                         {75, 75, 75}}, AxisAlignedBoundingBox3i{{0,   0,   0},
                                                                                                 {100, 100, 100}},
                                AxisAlignedBoundingBox3i{{0,   0,   0},
                                                         {100, 100, 100}}), // a inside b
                std::make_tuple(AxisAlignedBoundingBox3i{{0, 0, 0},
                                                         {1, 1, 1}}, AxisAlignedBoundingBox3i{{1, 0, 0},
                                                                                              {2, 1, 1}},
                                AxisAlignedBoundingBox3i{{0, 0, 0},
                                                         {2, 1, 1}}), // a and b next to each other
                std::make_tuple(AxisAlignedBoundingBox3i{{0, 0, 0},
                                                         {1, 1, 1}}, AxisAlignedBoundingBox3i{{2, 0, 0},
                                                                                              {3, 1, 1}},
                                AxisAlignedBoundingBox3i{{0, 0, 0},
                                                         {3, 1, 1}}),
                std::make_tuple(AxisAlignedBoundingBox3i{{0, 0, 0},
                                                         {1, 1, 1}}, AxisAlignedBoundingBox3i{{0, 2, 0},
                                                                                              {1, 3, 1}},
                                AxisAlignedBoundingBox3i{{0, 0, 0},
                                                         {1, 3, 1}}),
                std::make_tuple(AxisAlignedBoundingBox3i{{0, 0, 0},
                                                         {1, 1, 1}}, AxisAlignedBoundingBox3i{{2, 2, 0},
                                                                                              {3, 3, 1}},
                                AxisAlignedBoundingBox3i{{0, 0, 0},
                                                         {3, 3, 1}}),
                std::make_tuple(AxisAlignedBoundingBox3i{{-1, -1, -1},
                                                         {0,  0,  0}}, AxisAlignedBoundingBox3i{{2, 2, 2},
                                                                                                {3, 3, 3}},
                                AxisAlignedBoundingBox3i{{-1, -1, -1},
                                                         {3,  3,  3}})
        )
);

TEST(AxisAlignedBoundingBox2i, contains) {
    Point2i min_v{0, 0};
    Point2i max_v{100, 100};
    AxisAlignedBoundingBox2i bounds{min_v, max_v};

    EXPECT_THAT(bounds.contains(Point2i{-100, 0}), false);
    EXPECT_THAT(bounds.contains(Point2i{200, 0}), false);
}
