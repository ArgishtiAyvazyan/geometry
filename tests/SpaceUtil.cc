/**
 * @file        SpaceUtil.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Tests implementation for unti tests for space::utils.
 * @date        20-03-2021
 * @copyright   Copyright (c) 2021
 */

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>

#include <boost/geometry/index/rtree.hpp>


#include "Rect.h"
#include "Square.h"
#include "SimplePolygon.h"
#include "Polygon.h"
#include "Utility.h"

template <typename TCrd>
auto spaceToBoostRect(const space::Rect<TCrd>& rect)
{
    using point = boost::geometry::model::point<TCrd, 2, boost::geometry::cs::cartesian>;
    using box = boost::geometry::model::box<point>;

    const auto[x1, y1] = space::util::bottomLeft(rect);
    const auto[x2, y2] = space::util::topRight(rect);
    return box(point(x1, y1), point(x2, y2));
}

TEST_CASE("Cover::Rect::point", "[space::util]")
{
    space::Rect<int32_t> rect {{0, 0}, 100, 100};
    space::Point<int32_t> point {50, 50};
    REQUIRE (space::util::contains(rect, point));
    space::util::move(point, 100, 100);
    REQUIRE_FALSE (space::util::contains(rect, point));
}

TEST_CASE("Cover::Rect", "[space::util]")
{
    space::Rect<int32_t> rect1 {{0, 0}, 100, 100};
    space::Rect<int32_t> rect2 {{50, 50}, 10, 10};
    REQUIRE (space::util::contains(rect1, rect2));
    space::util::move(rect2, 100, 100);
    REQUIRE_FALSE (space::util::contains(rect1, rect2));
}

TEST_CASE("Cover::Square::point", "[space::util]")
{
    space::Square<int32_t> rect {{0, 0}, 100};
    space::Point<int32_t> point {50, 50};
    REQUIRE (space::util::contains(rect, point));
    space::util::move(point, 100, 100);
    REQUIRE_FALSE (space::util::contains(rect, point));
}

TEST_CASE("Cover::Square", "[space::util]")
{
    space::Square<int32_t> rect1 {{0, 0}, 100};
    space::Rect<int32_t> rect2 {{50, 50}, 10, 10};
    REQUIRE (space::util::contains(rect1, rect2));
    space::util::move(rect2, 100, 100);
    REQUIRE_FALSE (space::util::contains(rect1, rect2));
}

TEST_CASE("Intersects::Rect", "[space::util]")
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

TEST_CASE("Intersects::Square", "[space::Rect]")
{
    space::Rect<int32_t> rect {{50, 13}, 100, 100};
    space::Square<int32_t> rect1 {{0, 0}, 123};

    REQUIRE(space::util::hesIntersect(rect, rect1));
    REQUIRE(space::util::hesIntersect(rect1, rect));

    space::util::move(rect1, 149, 110);

    REQUIRE(space::util::hesIntersect(rect, rect1));
    REQUIRE(space::util::hesIntersect(rect1, rect));

    space::util::move(rect1.pos(), 100000, 100000);

    REQUIRE_FALSE(space::util::hesIntersect(rect, rect1));
    REQUIRE_FALSE(space::util::hesIntersect(rect1, rect));
}

TEST_CASE("empty::SimplePolygon", "[space::SimplePolygon]")
{
    using Poly = space::SimplePolygon<int32_t>;
    Poly poly;
    REQUIRE (poly.empty());
    Poly::TPiecewiseLinearCurve boundary {{0, 0},
                                          {1, 1},
                                          {2, 2}};
    Poly poly1 {boundary};
    REQUIRE_FALSE (poly1.empty());
}

TEST_CASE("boundary::SimplePolygon", "[space::SimplePolygon]")
{
    using Poly = space::SimplePolygon<int32_t>;
    Poly poly;
    REQUIRE_THROWS (poly.boundary());

    Poly::TPiecewiseLinearCurve boundary {{0, 0},
                                          {1, 1},
                                          {2, 2}};

    Poly poly1 {boundary};
    REQUIRE(boundary == poly1.boundary());
}


TEST_CASE("empty::Polygon", "[space::Polygon]")
{
    using Poly = space::Polygon<int32_t>;
    using SimplePoly = space::SimplePolygon<int32_t>;
    Poly poly;
    REQUIRE (poly.empty());
    REQUIRE_FALSE (poly.hasHoles());
    Poly::TSimplePolugon boundary {{{0, 0},
                                       {1, 1},
                                       {2, 2}}};
    Poly poly1 {boundary};
    REQUIRE_FALSE (poly1.empty());
    REQUIRE_FALSE (poly1.hasHoles());

    space::Vector<SimplePoly> holes;
    holes.push_back(SimplePoly {{{3, 3}, {1, 1}, {2, 2}}});
    holes.push_back(SimplePoly {{{6, 6}, {3, 3}, {9, 9}}});

    Poly poly2 {boundary, holes};
    REQUIRE_FALSE (poly2.empty());
    REQUIRE (poly2.hasHoles());
}

TEST_CASE("hasHoles::Polygon", "[space::Polygon]")
{
    using Poly = space::Polygon<int32_t>;
    using SimplePoly = space::SimplePolygon<int32_t>;

    Poly poly;
    REQUIRE_FALSE (poly.hasHoles());
    Poly::TSimplePolugon boundary {{{0, 0},
                                       {1, 1},
                                       {2, 2}}};
    space::Vector<SimplePoly> holes;
    holes.push_back(SimplePoly {{{3, 3}, {1, 1}, {2, 2}}});
    holes.push_back(SimplePoly {{{6, 6}, {3, 3}, {9, 9}}});

    Poly poly1 {boundary, holes};
    REQUIRE_FALSE (poly1.empty());
    REQUIRE (poly1.hasHoles());
}

TEST_CASE("boundary::Polygon", "[space::Polygon]")
{
    using Poly = space::Polygon<int32_t>;

    Poly poly;
    REQUIRE_THROWS (poly.boundary());

    Poly::TSimplePolugon boundary {{{0, 0},
                                       {1, 1},
                                       {2, 2}}};

    Poly poly1 {boundary};
    REQUIRE(boundary == poly1.boundary());
}

TEST_CASE("holes::Polygon", "[space::Polygon]")
{
    using Poly = space::Polygon<int32_t>;
    using SimplePoly = space::SimplePolygon<int32_t>;

    Poly poly;
    REQUIRE (poly.holes().empty());

    Poly::TSimplePolugon boundary {{{0, 0},
                                       {1, 1},
                                       {2, 2}}};
    Poly poly1 {boundary};
    REQUIRE (poly1.holes().empty());

    space::Vector<SimplePoly> holes;
    holes.push_back(SimplePoly {{{3, 3}, {1, 1}, {2, 2}}});
    holes.push_back(SimplePoly {{{6, 6}, {3, 3}, {9, 9}}});

    Poly poly2 {boundary, holes};
    auto spanHoles = poly2.holes();
    REQUIRE_FALSE (spanHoles.empty());
    REQUIRE(std::size(spanHoles) == std::size(holes));
}