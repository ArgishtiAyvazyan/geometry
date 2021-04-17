#pragma once

#include <random>
#include <algorithm>
#include <iostream>

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/index/rtree.hpp>

#include <catch2/catch.hpp>

#include "Rect.h"
#include "Square.h"
#include "QuadTree.h"
#include "Utility.h"

namespace test_util
{

template <typename TCrd>
auto spaceToBoostRect(const space::Rect<TCrd>& rect)
{
    using point = boost::geometry::model::point<TCrd, 2, boost::geometry::cs::cartesian>;
    using box = boost::geometry::model::box<point>;

    const auto[x1, y1] = space::util::bottomLeftOf(rect);
    const auto[x2, y2] = space::util::topRightOf(rect);
    return box(point(x1, y1), point(x2, y2));
}

template <typename TCrd>
auto boostToSpaceRect(
    const boost::geometry::model::box<boost::geometry::model::point<TCrd, 2, boost::geometry::cs::cartesian>>& rect)
{
    auto x1_ = rect.min_corner().template get<0>();
    auto y1_ = rect.min_corner().template get<1>();
    auto x2_ = rect.max_corner().template get<0>();
    auto y2_ = rect.max_corner().template get<1>();
    space::Rect<TCrd> r {{x1_, y1_}, (x2_ - x1_), (y2_ - y1_)};
    return r;
}

template <typename TCrd>
void printDiff(
    const std::vector<space::Rect<TCrd>>& quadTreeQueryRes,
    const std::vector<std::pair<boost::geometry::model::box<boost::geometry::model::point<TCrd, 2, boost::geometry::cs::cartesian>>, unsigned>>& rTreeQueryRes)
{
    std::cout << "=============================================" << std::endl;
    std::set<space::Rect<TCrd>> rectsSet(quadTreeQueryRes.begin(), quadTreeQueryRes.end());

    for (const auto& item : rTreeQueryRes)
    {
        auto r = boostToSpaceRect(item.first);
        if (rectsSet.find(r) == rectsSet.end())
        {
            std::cout << r << " Index: " << item.second << std::endl;
        }
    }
}

int rand(int from, int to)
{
    return (std::rand() % (to - from)) + from;
}

template <typename TCrd>
auto getRandPoint(TCrd maxPos)
{
    return space::Point {rand(0, maxPos), rand(0, maxPos)};
}

template <typename TCrd>
auto getRandRect(TCrd maxPos, TCrd maxRectWidth, TCrd maxRectHeight)
{
    return space::Rect<TCrd> {getRandPoint(maxPos), rand(0, maxRectWidth), rand(0, maxRectHeight)};
}

template <typename TIndex, typename TCrd, size_t Count>
void queryTest(TCrd maxPos, TCrd maxRectWidth, TCrd maxRectHeight)
{
    std::set<space::Rect<TCrd>> initialRects;
    for (size_t i = 0; i < Count; ++i)
    {
        initialRects.insert(getRandRect(maxPos, maxRectWidth, maxRectHeight));
    }

    using point = boost::geometry::model::point<TCrd, 2, boost::geometry::cs::cartesian>;
    using box = boost::geometry::model::box<point>;
    using value = std::pair<box, unsigned>;

    boost::geometry::index::rtree<value, boost::geometry::index::quadratic<16> > rtree;
    TIndex index;

    for (const auto& rect : initialRects)
    {
        REQUIRE_FALSE(index.contains(rect));
        REQUIRE(index.insert(rect));
        REQUIRE(index.contains(rect));
        box b = spaceToBoostRect(rect);
        rtree.insert(std::make_pair(b, 0));
        REQUIRE(index.size() == rtree.size());
    }

    for (size_t i = 0; i < Count; ++i)
    {
        space::Rect<TCrd> queryRect {getRandRect(maxPos, maxRectWidth, maxRectHeight)};

        std::vector<space::Rect<TCrd>> quadTreeQueryRes;
        index.query(queryRect, std::back_inserter(quadTreeQueryRes));


        const auto[x1, y1] = space::util::bottomLeftOf(queryRect);
        const auto[x2, y2] = space::util::topRightOf(queryRect);
        box queryBox(point(x1, y1), point(x2, y2));
        std::vector<value> rTreeQueryRes;
        rtree.query(boost::geometry::index::intersects(queryBox), std::back_inserter(rTreeQueryRes));

        if (std::size(quadTreeQueryRes) != std::size(rTreeQueryRes))
        {
            std::cout << "Query rect: " << queryRect << ": Index : " << i << std::endl;
            printDiff(quadTreeQueryRes, rTreeQueryRes);
        }
        REQUIRE(std::size(quadTreeQueryRes) == std::size(rTreeQueryRes));
    }
}

template <typename TIndex, typename TCrd, size_t Count>
void removeTest(TCrd maxPos, TCrd maxRectWidth, TCrd maxRectHeight)
{
    TIndex index;
    size_t size = 0;
    for (size_t i = 0; i < Count; ++i)
    {
        REQUIRE(index.size() == size);
        if (index.insert(getRandRect(maxPos, maxRectWidth, maxRectHeight)))
        {
            ++size;
        }
    }

    for (size_t i = 0; i < Count; ++i)
    {
        space::Rect<TCrd> removeRect {getRandRect(maxPos, maxRectWidth, maxRectHeight)};

        std::vector<space::Rect<TCrd>> quadTreeQueryRes;
        index.query(removeRect, std::back_inserter(quadTreeQueryRes));
        for (auto& rect : quadTreeQueryRes)
        {
            REQUIRE(index.contains(rect));
            index.remove(rect);
            REQUIRE_FALSE(index.contains(rect));
        }
        size -= quadTreeQueryRes.size();
        REQUIRE(index.size() == size);
        std::vector<space::Rect<TCrd>> quadTreeQueryRes2;
        index.query(removeRect, std::back_inserter(quadTreeQueryRes2));
        REQUIRE(quadTreeQueryRes2.empty());
    }
}


template <typename TIndex, typename TCrd>
void actionsOnEmptyIndexTest()
{
    TIndex index;
    space::Rect<TCrd> rect {{13, 13}, 13, 13};

    REQUIRE_FALSE(index.contains(rect));
    std::vector<space::Rect<TCrd>> quadTreeQueryRes;
    index.query(rect, std::back_inserter(quadTreeQueryRes));
    REQUIRE(quadTreeQueryRes.empty());
    index.remove(rect);
}

template <typename TIndex, typename TCrd>
void emptyIndexTest()
{
    TIndex index;
    REQUIRE(index.empty());
    space::Rect<TCrd> rect {{13, 13}, 13, 13};
    index.insert(rect);
    REQUIRE_FALSE(index.empty());
    index.remove({{14, 13}, 13, 13});
    REQUIRE_FALSE(index.empty());
    index.remove({{13, 13}, 13, 13});
    REQUIRE(index.empty());
}

template <typename TIndex, typename TCrd>
void clearIndexTest()
{
    TIndex index;
    REQUIRE(index.empty());
    for (size_t i = 0; i < 100; ++i)
    {
        index.insert(getRandRect(100, 100, 100));
    }
    REQUIRE_FALSE(index.empty());
    index.clear();
    REQUIRE(index.empty());
}

template <typename TIndex, typename TCrd, size_t Count>
void sizeTest(TCrd maxPos, TCrd maxRectWidth, TCrd maxRectHeight)
{
    std::set<space::Rect<TCrd>> initialRects;
    for (size_t i = 0; i < Count; ++i)
    {
        initialRects.insert(getRandRect(maxPos, maxRectWidth, maxRectHeight));
    }

    TIndex index;
    size_t size = 0;
    for (const auto& rect : initialRects)
    {
        REQUIRE(index.size() == size);
        ++size;
        index.insert(rect);
    }

    for (const auto& rect : initialRects)
    {
        REQUIRE(index.size() == size);
        --size;
        index.remove(rect);
    }
}

} // namespace test_util
