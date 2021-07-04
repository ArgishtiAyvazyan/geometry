/**
 * @file        QuadTree.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Tests implementation for unti tests for space::QuadTree.
 * @date        25-03-2021
 * @copyright   Copyright (c) 2021
 */


#include <gtest/gtest.h>


#include "IndexTestingUtils.h"


TEST(QuadTreForFoints, space_QuadTree)
{
    using value_type = int32_t;
    test_util::queryTest<space::QuadTree<space::Rect<value_type>>, value_type, 10'000>(1'000, 1, 1);
}

TEST(QuadTreeSimpleTest, space_QuadTree)
{
    using value_type = int32_t;
    test_util::queryTest<space::QuadTree<space::Rect<value_type>>, value_type, 10'000>(1'000, 1'000, 1'000);
}

TEST(QuadTreeStresTest, space_QuadTree)
{
    using value_type = int32_t;
    test_util::queryTest<space::QuadTree<space::Rect<value_type>>, value_type, 1'000>(1'000, 1, 1'000);
    test_util::queryTest<space::QuadTree<space::Rect<value_type>>, value_type, 1'000>(1'000, 1'000, 1);
    test_util::queryTest<space::QuadTree<space::Rect<value_type>>, value_type, 1'000>(
        1'000, 1'000'000, 1'000'000);
}

TEST(QuadTreeRemove, space_QuadTree)
{
    using value_type = int32_t;
    test_util::removeTest<space::QuadTree<space::Rect<value_type>>, value_type, 1'000>(1'000, 1'000, 1'000);
    test_util::removeTest<space::QuadTree<space::Rect<value_type>>, value_type, 1'000>(1'000, 1, 1'000);
    test_util::removeTest<space::QuadTree<space::Rect<value_type>>, value_type, 1'000>(1'000, 1'000, 1);
    test_util::removeTest<space::QuadTree<space::Rect<value_type>>, value_type, 1'000>(
        1'000, 1'000'000, 1'000'000);
}

TEST(QuadTreeActionsOnEmptyTree, space_QuadTree)
{
    using value_type = int32_t;
    test_util::actionsOnEmptyIndexTest<space::QuadTree<space::Rect<value_type>>, value_type>();
}

TEST(QuadTreeEmpty, space_QuadTree)
{
    using value_type = int32_t;
    test_util::emptyIndexTest<space::QuadTree<space::Rect<value_type>>, value_type>();
}

TEST(QuadTreeClear, space_QuadTree)
{
    using value_type = int32_t;
    test_util::clearIndexTest<space::QuadTree<space::Rect<value_type>>, value_type>();
}

TEST(QuadTreeSize, space_QuadTree)
{
    using value_type = int32_t;
    test_util::sizeTest<space::QuadTree<space::Rect<value_type>>, value_type, 10'000>(1'000, 1'000, 1'000);
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
