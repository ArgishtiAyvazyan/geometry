/**
 * @file        QuadTree.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Tests implementation for unti tests for space::QuadTree.
 * @date        25-03-2021
 * @copyright   Copyright (c) 2021
 */

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "IndexTestingUtils.h"



TEST_CASE("QuadTree for points", "[space::QuadTree]")
{
    using value_type = int32_t;
    test_util::queryTest<space::QuadTree<space::Rect<value_type>>, value_type, 10'000>(1'000, 1, 1);
}

TEST_CASE("QuadTree simple test.", "[space::QuadTree]")
{
    using value_type = int32_t;
    test_util::queryTest<space::QuadTree<space::Rect<value_type>>, value_type, 10'000>(1'000, 1'000, 1'000);
}

TEST_CASE("QuadTree stres test.", "[space::QuadTree]")
{
    using value_type = int32_t;
    test_util::queryTest<space::QuadTree<space::Rect<value_type>>, value_type, 1'000>(1'000, 1, 1'000);
    test_util::queryTest<space::QuadTree<space::Rect<value_type>>, value_type, 1'000>(1'000, 1'000, 1);
    test_util::queryTest<space::QuadTree<space::Rect<value_type>>, value_type, 1'000>(
        1'000, 1'000'000, 1'000'000);
}

TEST_CASE("QuadTree remove.", "[space::QuadTree]")
{
    using value_type = int32_t;
    test_util::removeTest<space::QuadTree<space::Rect<value_type>>, value_type, 1'000>(1'000, 1'000, 1'000);
    test_util::removeTest<space::QuadTree<space::Rect<value_type>>, value_type, 1'000>(1'000, 1, 1'000);
    test_util::removeTest<space::QuadTree<space::Rect<value_type>>, value_type, 1'000>(1'000, 1'000, 1);
    test_util::removeTest<space::QuadTree<space::Rect<value_type>>, value_type, 1'000>(
        1'000, 1'000'000, 1'000'000);
}

TEST_CASE("QuadTree actions on empty tree", "[space::QuadTree]")
{
    using value_type = int32_t;
    test_util::actionsOnEmptyIndexTest<space::QuadTree<space::Rect<value_type>>, value_type>();
}

TEST_CASE("QuadTree empty", "[space::QuadTree]")
{
    using value_type = int32_t;
    test_util::emptyIndexTest<space::QuadTree<space::Rect<value_type>>, value_type>();
}
