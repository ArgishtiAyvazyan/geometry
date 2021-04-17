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
 * @tparam  TCrd The type of coordinate.
 */
template <typename TCrd>
class Polygon
{
public:

    /**
     * @brief   The type of coordinate.
     */
    using TCoordinate = TCrd;

    /**
     * @brief   The type simple polygon.
     */
    using TSimplePolugon = space::SimplePolygon<TCoordinate>;
public:


    constexpr Polygon() = default;

    constexpr ~Polygon() = default;

    constexpr Polygon(Polygon&&) noexcept = default;

    constexpr Polygon(const Polygon&) noexcept = default;

    constexpr Polygon& operator=(Polygon&&) noexcept = default;

    constexpr Polygon& operator=(const Polygon&) noexcept = default;

    /**
     * @brief           Initializes a new instance of the Polygon structure
     *                  that has the specified external boundary and interior boundaries (holes).
     *
     * @param boundary  A simple polygon representing an external boundary for a polygon with holes.
     * @param holes     A set of simple polygon representing an interior boundaries (holes) for a polygon with holes.
     */
    constexpr explicit Polygon(TSimplePolugon boundary, space::Vector<TSimplePolugon> holes = {})
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
    const TSimplePolugon& boundary() const
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
    TSimplePolugon& boundary()
    {
        return const_cast<TSimplePolugon&>(std::as_const(*this).boundary());
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
    space::Span<const TSimplePolugon> holes() const noexcept
    {
        if (!hasHoles())
        {
            return {};
        }
        return space::Span<const TSimplePolugon> {m_arrContours}.subspan(1, std::size(m_arrContours) - 1);
    }

    /**
     * @brief   Gets the span to the interior boundaries (holes) collection.
     *          If the polygon hasn't holes returns empty span.
     *
     * @return  The span on the holes collection.
     */
    [[nodiscard]]
    space::Span<TSimplePolugon> holes() noexcept
    {
        if (!hasHoles())
        {
            return {};
        }
        return space::Span<TSimplePolugon> {m_arrContours}.subspan(1, std::size(m_arrContours) - 1);
    }


private:

    /**
     * @brief   The polygon representation by polygons. The first item represents
     *          the polygon outside boundaries the others represent holes.
     */
    space::Vector<TSimplePolugon> m_arrContours;
}; // class Polygon

namespace util
{
/**
 * @brief       Moves the Polygon by the specified horizontal and vertical amounts.
 *
 * @tparam TCrd The type of coordinates.
 * @param poly  The given polygon.
 * @param deltaX The horizontal amounts.
 * @param deltaY The vertical amounts.
 */
template <typename TCrd>
constexpr void move(Polygon<TCrd>& poly, TCrd deltaX, TCrd deltaY) noexcept
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
 * @tparam TCrd The type of coordinates.
 * @param  poly The given polygon.
 * @return      The boundary box (space::Rect) of the given polygon.
 */
template <typename TCrd>
constexpr space::Rect<TCrd> boundaryBoxOf(const Polygon<TCrd>& poly) noexcept
{
    return boundaryBoxOf(poly.boundary());
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
TOstream& operator<<(TOstream& os, const space::Polygon<TCrd>& poly)
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
