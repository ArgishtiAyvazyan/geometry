/**
 * @file        Polygon.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaring the Polygon class.
 * @date        03-04-2021
 * @copyright   Copyright (c) 2021
 */


#pragma once

#include <algorithm>

#include "Definitions.h"
#include "Point.h"
#include "SimplePolygon.h"

namespace space
{

/**
 * @class   Polygon
 * @brief   The c++ representation of polygon with holes.
 *          A polygon with holes is an area-connected simple polygon with one
 *          external boundary and one or more interior boundaries (holes).
 *
 * @tparam  TCrt The type of coordinate.
 */
template <typename TCrt>
class Polygon
{
public:

    /**
     * @brief   The type of coordinate.
     */
    using TCoordinate = TCrt;

    /**
     * @brief   The type simple polygon.
     */
    using TSimplePolygon = space::SimplePolygon<TCoordinate>;
public:


    constexpr Polygon() = default;

    constexpr ~Polygon() = default;

    constexpr Polygon(Polygon&&) noexcept = default;

    constexpr Polygon(const Polygon&) noexcept = default;

    constexpr Polygon& operator=(Polygon&&) noexcept = default;

    constexpr Polygon& operator=(const Polygon&) noexcept = default;

    /* constexpr */ std::strong_ordering operator<=>(const Polygon<TCrt>&) const noexcept = default;

    /**
     * @brief           Initializes a new instance of the Polygon structure
     *                  that has the specified external boundary and interior boundaries (holes).
     *
     * @param boundary  A simple polygon representing an external boundary for a polygon with holes.
     * @param holes     A set of simple polygon representing an interior boundaries (holes) for a polygon with holes.
     */
    constexpr explicit Polygon(TSimplePolygon boundary, space::Vector<TSimplePolygon> holes = {})
        : m_arrContours {}
    {
        m_arrContours.reserve(std::size(holes) + 1);
        m_arrContours.push_back(std::move(boundary));
        std::ranges::move(holes, std::back_inserter(m_arrContours));
    }


    /**
     * @brief   Checks the polygon has external boundary or not.
     *
     * @return  true if has, otherwise false.
     */
    [[nodiscard]]
    bool empty() const noexcept
    {
        return std::empty(m_arrContours);
    }

    /**
     * @brief   Gets the const reference to polygon external boundary polygon.
     *
     * @throws  std::out_of_range if the polygon is empty.
     *
     * @return  The const reference to polygon external boundary polygon.
     */
    [[nodiscard]]
    const TSimplePolygon& boundary() const
    {
        if (empty())
        {
            throw std::out_of_range {"The Polygon is empty."};
        }
        return m_arrContours.front();
    }

    /**
     * @brief   Gets the reference to polygon external boundary polygon.
     *
     * @throws  std::out_of_range if the polygon is empty.
     *
     * @return  The reference to polygon external boundary polygon.
     */
    [[nodiscard]]
    TSimplePolygon& boundary()
    {
        return const_cast<TSimplePolygon&>(std::as_const(*this).boundary());
    }

    /**
     * @brief   Checks the polygon has interior boundaries (holes) or not.
     *
     * @return  true if has, otherwise false.
     */
    [[nodiscard]]
    bool hasHoles() const noexcept
    {
        return std::size(m_arrContours) > 1;
    }

    /**
     * @brief   Gets the const span to the interior boundaries (holes) collection.
     *          If the polygon hasn't holes returns empty span.
     *
     * @return  The const span on the holes collection.
     */
    [[nodiscard]]
    space::Span<const TSimplePolygon> holes() const noexcept
    {
        if (!hasHoles())
        {
            return {};
        }
        return space::Span<const TSimplePolygon> {m_arrContours}.subspan(1, std::size(m_arrContours) - 1);
    }

    /**
     * @brief   Gets the span to the interior boundaries (holes) collection.
     *          If the polygon hasn't holes returns empty span.
     *
     * @return  The span on the holes collection.
     */
    [[nodiscard]]
    space::Span<TSimplePolygon> holes() noexcept
    {
        if (!hasHoles())
        {
            return {};
        }
        return space::Span<TSimplePolygon> {m_arrContours}.subspan(1, std::size(m_arrContours) - 1);
    }


private:

    /**
     * @brief   The polygon representation by polygons. The first item represents
     *          the polygon outside boundaries the others represent holes.
     */
    space::Vector<TSimplePolygon> m_arrContours;
}; // class Polygon

namespace util
{
/**
 * @brief       Moves the Polygon by the specified horizontal and vertical amounts.
 *
 * @tparam TCrt The type of coordinates.
 * @param poly  The given polygon.
 * @param deltaX The horizontal amounts.
 * @param deltaY The vertical amounts.
 */
template <typename TCrt>
constexpr void move(Polygon<TCrt>& poly, TCrt deltaX, TCrt deltaY) noexcept
{
    move(poly.boundary(), deltaX, deltaY);
    auto movePolygon = [deltaX, deltaY](auto& simplePoly)
    {
        move(simplePoly, deltaX, deltaY);
    };
    std::ranges::for_each(poly.holes(), std::ref(movePolygon));
}

/**
 * @brief       Gets the boundary box of the given polygon.
 *
 * @details     The algorithm complexity is O(n).
 *
 * @tparam TCrt The type of coordinates.
 * @param  poly The given polygon.
 * @return      The boundary box (space::Rect) of the given polygon.
 */
template <typename TCrt>
constexpr space::Rect<TCrt> boundaryBoxOf(const Polygon<TCrt>& poly) noexcept
{
    return boundaryBoxOf(poly.boundary());
}

/**
 * @brief   Returns true if the given point is inside or on the edge of the Polygon,
 *          otherwise returns false.
 *
 * @details Calculates whether a point is inside a boundary simple polygon and outside of holes.
 *          The running time linearly depends on the number of vertices in the polygon and holes.
 * @tparam  TCrt The type of coordinates.
 * @param   poly The given Polygon.
 * @param   point The given point.
 * @return  true if contains, otherwise returns false.
 */
template <typename TCrt>
[[nodiscard]]
constexpr bool contains(const Polygon<TCrt>& poly, const Point<TCrt>& point) noexcept
{
    if (poly.empty() || !contains(poly.boundary(), point))
    {
        return false;
    }

    return std::ranges::none_of(poly.holes()
        , [point](const auto& simplePolygon)
        {
           return contains(simplePolygon, point);
        });
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
TOstream& operator<<(TOstream& os, const space::Polygon<TCrt>& poly)
{
    os << "Polygon { Boundary: { ";
    os << poly.boundary();
    if (poly.hasHoles())
    {
        for (auto& hole : poly.holes())
        {
            os << " } Hole: { ";
            os << hole;
        }
    }
    os << " } }";
    return os;
}

} // namespace space
