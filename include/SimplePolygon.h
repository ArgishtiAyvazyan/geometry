/**
 * @file        SimplePolygon.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaring the SimplePolygon class.
 * @date        21-04-2021
 * @copyright   Copyright (c) 2021
 */

#pragma once

#include <algorithm>
#include <iterator>
#include <optional>
#include <iostream>

#include "Definitions.h"
#include "Point.h"
#include "Rect.h"
#include "Segment.h"

namespace space
{

/**
 * @class   SimplePolygon
 * @brief   The c++ representation of simple polygon.
 *          A simple polygon is a subset of the plane bounded with
 *          a non-self-intersecting piecewise linear curve.
 *
 * @details The piecewise linear curve represented by a set of points.
 *          Points in simple polygon are always in clockwise order.
 *
 * @tparam  TCrt The type of coordinate.
 */
template <typename TCrt>
class SimplePolygon
{
public:
    /**
     * @brief   The type of coordinate.
     */
    using TCoordinate = TCrt;

    /**
     * @brief   The type for representing piecewise linear curve.
     */
    using TPiecewiseLinearCurve = space::Vector<space::Point<TCoordinate>>;

public:
    constexpr SimplePolygon() = default;

    constexpr ~SimplePolygon() = default;

    constexpr SimplePolygon(SimplePolygon&&) noexcept = default;

    constexpr SimplePolygon(const SimplePolygon&) noexcept = default;

    constexpr SimplePolygon& operator=(SimplePolygon&&) noexcept = default;

    constexpr SimplePolygon& operator=(const SimplePolygon&) noexcept = default;

    /* constexpr */ std::strong_ordering operator<=>(const SimplePolygon&) const noexcept = default;

    /**
     * @brief   Initializes a new instance of the SimplePolygon structure
     *          that has the specified piecewise linear curve.
     *
     * @param   piecewiseLinearCurve The set of points represents a piecewise
     *          linear curve.
     */
    constexpr explicit SimplePolygon(TPiecewiseLinearCurve piecewiseLinearCurve)
        : m_piecewiseLinearCurve {std::move(piecewiseLinearCurve)}
    {
    }


    /**
     * @brief   Checks the polygon has points or not.
     *
     * @return  true if has, otherwise false.
     */
    [[nodiscard]]
    bool empty() const noexcept
    {
        return std::empty(m_piecewiseLinearCurve);
    }

    /**
     * @brief   Gets the const reference to polygon boundary (piecewise linear curve).
     *
     * @throws  std::out_of_range if the polygon is empty.
     *
     * @return  The const reference to polygon boundary (piecewise linear curve).
     */
    [[nodiscard]]
    const TPiecewiseLinearCurve& boundaryCurve() const
    {
        if (empty())
        {
            throw std::out_of_range {"The Polygon is empty."};
        }
        return m_piecewiseLinearCurve;
    }

    /**
     * @brief   Gets the reference to polygon boundary (piecewise linear curve).
     *
     * @throws  std::out_of_range if the polygon is empty.
     *
     * @return  The reference to polygon boundary (piecewise linear curve).
     */
    [[nodiscard]]
    TPiecewiseLinearCurve& boundaryCurve()
    {
        return const_cast<TPiecewiseLinearCurve&>(std::as_const(*this).boundaryCurve());
    }

private:

    /**
     * @brief   The piecewise linear curve representing by a set of points.
     */
    TPiecewiseLinearCurve m_piecewiseLinearCurve {};
}; // class SimplePolygon


namespace util
{
/**
 * @brief       Moves the simple polygon by the specified horizontal and vertical amounts.
 *
 * @tparam TCrt The type of coordinates.
 * @param poly  The given simple polygon.
 * @param deltaX The horizontal amounts.
 * @param deltaY The vertical amounts.
 */
template <typename TCrt>
constexpr void move(SimplePolygon<TCrt>& poly, TCrt deltaX, TCrt deltaY) noexcept
{
    auto movePoint = [deltaX, deltaY](auto& point)
    {
        move(point, deltaX, deltaY);
    };
    std::ranges::for_each(poly.boundaryCurve(), std::ref(movePoint));
}

/**
 * @brief       Gets the boundary box of the given simple polygon.
 *
 * @details     The algorithm complexity is O(n).
 *
 * @tparam TCrt The type of coordinates.
 * @param poly  The given polygon.
 * @return      The boundary box (space::Rect) of the given polygon.
 */
template <typename TCrt>
constexpr space::Rect<TCrt> boundaryBoxOf(const SimplePolygon<TCrt>& poly) noexcept
{
    auto[leftBottomIt, rightTopIt] = std::ranges::minmax_element(poly.boundaryCurve());
    return space::Rect<TCrt>(*leftBottomIt, *rightTopIt);
}

namespace impl
{
/**
 * @internal
 * @brief       Returns number that can be used as infinity for the given polygon.
 *
 * @details     Finds the biggest x-axis coordinate in a polygon, increments it by 1, then returns.
 * @tparam TCrt The type of coordinates.
 * @param poly  The given polygon.
 * @return      Pseudo infinity number.
 */
template <typename TCrt>
TCrt infinityXAxisFor(const SimplePolygon<TCrt>& poly) noexcept
{
    space::Rect boundingBox = space::util::boundaryBoxOf(poly);
    return space::util::topRightOf(boundingBox).x() + 1;
}
} // namespace impl

/**
 * @brief   Returns true if the given point is inside or on the edge of the Simple Polygon,
 *          otherwise returns false.
 *
 * @details Calculates whether a point is inside a simple polygon. A polygon is defined by a
 *          sequence of points (boundary curve). Staying inside is determined by the even-odd rule.
 *          If we take a ray that starts at a point and goes off to infinity (in any direction),
 *          we count the number of intersections. If this number is odd, the point is inside;
 *          otherwise, it is outside. The running time linearly depends on the number of vertices
 *          in the polygon.
 * @tparam  TCrt The type of coordinates.
 * @param   poly The given Simple Polygon.
 * @param   point The given point.
 * @return  true if contains, otherwise returns false.
 */
template <typename TCrt>
[[nodiscard]]
constexpr bool contains(const SimplePolygon<TCrt>& poly, const Point<TCrt>& point) noexcept
{
    // Create a horizontal line from point to infinity.
    const auto infinity = impl::infinityXAxisFor(poly);
    const Segment<TCrt> horizontalLine { point, Point<TCrt>{infinity, point.y()} };

    // Count intersections of the above line with sides of polygon
    size_t count = 0;
    size_t i = 0;

    const auto& boundary = poly.boundaryCurve();
    auto numOfVertex = std::size(boundary);

    if (numOfVertex < 3)
    {
        return false;
    }

    do
    {
        const auto next = (i + 1) % numOfVertex;
        Segment polygonEdge {boundary[i], boundary[next]};
        if (hesIntersect(polygonEdge, horizontalLine))
        {
            if (impl::EOrientation::collinear
                == impl::orientation(polygonEdge.first, point, polygonEdge.second))
            {
                return impl::onSegment(polygonEdge, point);
            }
            if (impl::onSegment(horizontalLine, polygonEdge.second))
            {
                if (impl::orientation(point, polygonEdge.second, polygonEdge.first)
                    == impl::orientation(boundary[(i + 2) % numOfVertex], polygonEdge.second, point))
                {
                    ++count;
                }
            }
            ++count;
        }

        i = next;
    } while (i != 0);

    // Return true if count is odd, false otherwise
    return count & 1;
}

} // namespace util


/**
 * @brief   The ostream operators for working with streams.
 *
 * @note    Templates used to avoid include stream headers.
 *
 * @tparam  TOstream The type of ostream.
 * @tparam  TCrt The type of coordinates.
 * @param   os The ostream.
 * @param   point The Rect.
 * @return  The reference to ostream.
 */
template <typename TOstream, typename TCrt>
TOstream& operator<<(TOstream& os, const space::SimplePolygon<TCrt>& poly)
{
    os << "SimplePolygon { ";
    std::ranges::copy(poly.boundaryCurve(), std::ostream_iterator<space::Point<TCrt>>(os, ", "));
    os << "}";
    return os;
}

} // namespace space
