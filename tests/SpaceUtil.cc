/**
 * @file        SpaceUtil.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Tests implementation for unti tests for space::utils.
 * @date        20-02-2021
 * @copyright   Copyright (c) 2020
 */

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>

#include <boost/geometry/index/rtree.hpp>


#include "Rect.h"

template <typename TCrd>
auto spaceToBoostRect(const space::Rect<TCrd>& rect)
{
    using point = boost::geometry::model::point<TCrd, 2, boost::geometry::cs::cartesian>;
    using box = boost::geometry::model::box<point>;

    const auto[x1, y1] = space::util::bottomLeft(rect);
    const auto[x2, y2] = space::util::topRight(rect);
    return box(point(x1, y1), point(x2, y2));
}

TEST_CASE("Intersects", "[space::util]")
{
    space::Rect<int32_t> rect {{50, 13}, 100, 100};
    space::Rect<int32_t> rect1 {{0, 0}, 123, 123};

    REQUIRE(space::util::hesIntersect(rect, rect1));
    REQUIRE(space::util::hesIntersect(rect1, rect));

    space::util::move(rect1, 149, 110);

    REQUIRE(space::util::hesIntersect(rect, rect1));
    REQUIRE(space::util::hesIntersect(rect1, rect));

    space::util::move(rect1, 100000, 100000);

    REQUIRE_FALSE(space::util::hesIntersect(rect, rect1));
    REQUIRE_FALSE(space::util::hesIntersect(rect1, rect));

    for (int64_t i = 0; i < 1'000'000; ++i)
    {
        space::Rect<int32_t> spaceRect1 {{std::rand() % 1000, std::rand() % 1000}, std::rand() % 1000,
                                         std::rand() % 1000};
        space::Rect<int32_t> spaceRect2 {{std::rand() % 1000, std::rand() % 1000}, std::rand() % 1000,
                                         std::rand() % 1000};
        const auto boostBox1 = spaceToBoostRect(spaceRect1);
        const auto boostBox2 = spaceToBoostRect(spaceRect2);
        REQUIRE(space::util::hesIntersect(spaceRect1, spaceRect2)
                == boost::geometry::intersects(boostBox1, boostBox2));
    }
}

TEST_CASE("Cover::point", "[space::util]")
{
    space::Rect<int32_t> rect { {0, 0}, 100, 100 };
    space::Point<int32_t> point { 50, 50 };
    REQUIRE (space::util::contains(rect, point));
    space::util::move(point, 100, 100);
    REQUIRE_FALSE (space::util::contains(rect, point));
}

TEST_CASE("Cover::rect", "[space::util]")
{
    space::Rect<int32_t> rect1 { {0, 0}, 100, 100 };
    space::Rect<int32_t> rect2 { {50, 50}, 10, 10 };
    REQUIRE (space::util::contains(rect1, rect2));
    space::util::move(rect2, 100, 100);
    REQUIRE_FALSE (space::util::contains(rect1, rect2));
    std::cout << "Hello" << std::endl;
}
