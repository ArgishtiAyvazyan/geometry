#pragma once

#include <gtest/gtest.h>


#include <random>
#include <algorithm>
#include <iostream>

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/index/rtree.hpp>

#include "Rect.h"
#include "Square.h"
#include "QuadTree.h"
#include "Utility.h"

namespace test_util
{

template <typename TCrt>
auto spaceToBoostRect(const space::Rect<TCrt>& rect)
{
    using point = boost::geometry::model::point<TCrt, 2, boost::geometry::cs::cartesian>;
    using box = boost::geometry::model::box<point>;

    const auto[x1, y1] = space::util::bottomLeftOf(rect);
    const auto[x2, y2] = space::util::topRightOf(rect);
    return box(point(x1, y1), point(x2, y2));
}

template <typename TCrt>
auto boostToSpaceRect(
    const boost::geometry::model::box<boost::geometry::model::point<TCrt, 2, boost::geometry::cs::cartesian>>& rect)
{
    auto x1_ = rect.min_corner().template get<0>();
    auto y1_ = rect.min_corner().template get<1>();
    auto x2_ = rect.max_corner().template get<0>();
    auto y2_ = rect.max_corner().template get<1>();
    space::Rect<TCrt> r {{x1_, y1_}, (x2_ - x1_), (y2_ - y1_)};
    return r;
}

template <typename TCrt>
void printDiff(
    const std::vector<space::Rect<TCrt>>& quadTreeQueryRes,
    const std::vector<std::pair<boost::geometry::model::box<boost::geometry::model::point<TCrt, 2, boost::geometry::cs::cartesian>>, unsigned>>& rTreeQueryRes)
{
    std::cout << "=============================================" << std::endl;
    std::set<space::Rect<TCrt>> rectsSet(quadTreeQueryRes.begin(), quadTreeQueryRes.end());

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

template <typename TCrt>
auto getRandPoint(TCrt maxPos)
{
    return space::Point {rand(0, maxPos), rand(0, maxPos)};
}

template <typename TCrt>
auto getRandRect(TCrt maxPos, TCrt maxRectWidth, TCrt maxRectHeight)
{
    return space::Rect<TCrt> {getRandPoint(maxPos), rand(0, maxRectWidth), rand(0, maxRectHeight)};
}

template <typename TIndex, typename TCrt, size_t Count>
void queryTest(TCrt maxPos, TCrt maxRectWidth, TCrt maxRectHeight)
{
    std::set<space::Rect<TCrt>> initialRects;
    for (size_t i = 0; i < Count; ++i)
    {
        initialRects.insert(getRandRect(maxPos, maxRectWidth, maxRectHeight));
    }

    using point = boost::geometry::model::point<TCrt, 2, boost::geometry::cs::cartesian>;
    using box = boost::geometry::model::box<point>;
    using value = std::pair<box, unsigned>;

    boost::geometry::index::rtree<value, boost::geometry::index::quadratic<16> > rtree;
    TIndex index;

    for (const auto& rect : initialRects)
    {
        ASSERT_FALSE(index.contains(rect));
        ASSERT_TRUE(index.insert(rect));
        ASSERT_TRUE(index.contains(rect));
        box b = spaceToBoostRect(rect);
        rtree.insert(std::make_pair(b, 0));
        ASSERT_TRUE(index.size() == rtree.size());
    }

    for (size_t i = 0; i < Count; ++i)
    {
        space::Rect<TCrt> queryRect {getRandRect(maxPos, maxRectWidth, maxRectHeight)};

        std::vector<space::Rect<TCrt>> quadTreeQueryRes;
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
        ASSERT_TRUE(std::size(quadTreeQueryRes) == std::size(rTreeQueryRes));
    }
}

template <typename TIndex, typename TCrt, size_t Count>
void removeTest(TCrt maxPos, TCrt maxRectWidth, TCrt maxRectHeight)
{
    TIndex index;
    size_t size = 0;
    for (size_t i = 0; i < Count; ++i)
    {
        ASSERT_TRUE(index.size() == size);
        if (index.insert(getRandRect(maxPos, maxRectWidth, maxRectHeight)))
        {
            ++size;
        }
    }

    for (size_t i = 0; i < Count; ++i)
    {
        space::Rect<TCrt> removeRect {getRandRect(maxPos, maxRectWidth, maxRectHeight)};

        std::vector<space::Rect<TCrt>> quadTreeQueryRes;
        index.query(removeRect, std::back_inserter(quadTreeQueryRes));
        for (auto& rect : quadTreeQueryRes)
        {
            ASSERT_TRUE(index.contains(rect));
            index.remove(rect);
            ASSERT_FALSE(index.contains(rect));
        }
        size -= quadTreeQueryRes.size();
        ASSERT_TRUE(index.size() == size);
        std::vector<space::Rect<TCrt>> quadTreeQueryRes2;
        index.query(removeRect, std::back_inserter(quadTreeQueryRes2));
        ASSERT_TRUE(quadTreeQueryRes2.empty());
    }
}


template <typename TIndex, typename TCrt>
void actionsOnEmptyIndexTest()
{
    TIndex index;
    space::Rect<TCrt> rect {{13, 13}, 13, 13};

    ASSERT_FALSE(index.contains(rect));
    std::vector<space::Rect<TCrt>> quadTreeQueryRes;
    index.query(rect, std::back_inserter(quadTreeQueryRes));
    ASSERT_TRUE(quadTreeQueryRes.empty());
    index.remove(rect);
}

template <typename TIndex, typename TCrt>
void emptyIndexTest()
{
    TIndex index;
    ASSERT_TRUE(index.empty());
    space::Rect<TCrt> rect {{13, 13}, 13, 13};
    index.insert(rect);
    ASSERT_FALSE(index.empty());
    index.remove({{14, 13}, 13, 13});
    ASSERT_FALSE(index.empty());
    index.remove({{13, 13}, 13, 13});
    ASSERT_TRUE(index.empty());
}

template <typename TIndex, typename TCrt>
void clearIndexTest()
{
    TIndex index;
    ASSERT_TRUE(index.empty());
    for (size_t i = 0; i < 100; ++i)
    {
        index.insert(getRandRect(100, 100, 100));
    }
    ASSERT_FALSE(index.empty());
    index.clear();
    ASSERT_TRUE(index.empty());
}

template <typename TIndex, typename TCrt, size_t Count>
void sizeTest(TCrt maxPos, TCrt maxRectWidth, TCrt maxRectHeight)
{
    std::set<space::Rect<TCrt>> initialRects;
    for (size_t i = 0; i < Count; ++i)
    {
        initialRects.insert(getRandRect(maxPos, maxRectWidth, maxRectHeight));
    }

    TIndex index;
    size_t size = 0;
    for (const auto& rect : initialRects)
    {
        ASSERT_TRUE(index.size() == size);
        ++size;
        index.insert(rect);
    }

    for (const auto& rect : initialRects)
    {
        ASSERT_TRUE(index.size() == size);
        --size;
        index.remove(rect);
    }
}

} // namespace test_util
