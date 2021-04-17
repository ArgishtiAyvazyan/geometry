/**
 * @file        SimplePolygon.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaring the SimplePolygon class.
 * @date        21-04-2021
 * @copyright   Copyright (c) 2021
 */

#pragma once

#include <algorithm>

#include "Definitions.h"
#include "Point.h"
#include "Rect.h"

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
 * @tparam  TCrd The type of coordinate.
 */
template <typename TCrd>
class SimplePolygon
{
public:
    /**
     * @brief   The type of coordinate.
     */
    using TCoordinate = TCrd;

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
 * @tparam TCrd TCrd The type of coordinates.
 * @param poly  The given simple polygon.
 * @param deltaX The horizontal amounts.
 * @param deltaY The vertical amounts.
 */
template <typename TCrd>
constexpr void move(SimplePolygon<TCrd>& poly, TCrd deltaX, TCrd deltaY) noexcept
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
 * @tparam TCrd The type of coordinates.
 * @param poly  The given polygon.
 * @return      The boundary box (space::Rect) of the given polygon.
 */
template <typename TCrd>
constexpr space::Rect<TCrd> boundaryBoxOf(const SimplePolygon<TCrd>& poly) noexcept
{
    auto[leftBottomIt, rightTopIt] = std::ranges::minmax_element(poly.boundaryCurve());
    return space::Rect<TCrd>(*leftBottomIt, *rightTopIt);
}
} // namespace util

/**
 * @brief   The ostream operators for working with streams.
 *
 * @note    Templates used to avoid include stream headers.
 *
 * @tparam  TOstream The type of ostream.
 * @tparam  TCrd The type of coordinates.
 * @param   os The ostream.
 * @param   point The Rect.
 * @return  The reference to ostream.
 */
template <typename TOstream, typename TCrd>
TOstream& operator<<(TOstream& os, const space::SimplePolygon<TCrd>& poly)
{
    os << "SimplePolygon { ";
    std::ranges::copy(poly.boundaryCurve(), std::ostream_iterator<space::Point<TCrd>>(os, ", "));
    os << "}";
    return os;
}

} // namespace space
