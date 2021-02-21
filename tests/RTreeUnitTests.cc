/**
 * @file        CSVParserTest.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Tests implementation for CLIParser.
 * @date        20-02-2021
 * @copyright   Copyright (c) 2020
 */

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "Rect.h"

TEST_CASE( "Overlap", "[R-Tree::Rect]" )
{
    space::Rect<int32_t> rect {{ 50, 13 }, 100, 100};
    space::Rect<int32_t> rect1 {{0, 0}, 123, 123};

    REQUIRE(space::util::intersects(rect, rect1));
    REQUIRE(space::util::intersects(rect1, rect));

    space::util::move(rect1, 149, 110);

    REQUIRE(space::util::intersects(rect, rect1));
    REQUIRE(space::util::intersects(rect1, rect));

    space::util::move(rect1.pos(), 100000, 100000);

    REQUIRE_FALSE(space::util::intersects(rect, rect1));
    REQUIRE_FALSE(space::util::intersects(rect1, rect));
}

