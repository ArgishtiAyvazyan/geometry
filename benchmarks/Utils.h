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
    return space::Rect<TCrt> {getRandPoint(maxPos), rand(100, maxRectWidth), rand(100, maxRectHeight)};
}


}
