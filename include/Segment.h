/**
 * @file        Segment.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaring the Segment class.
 * @date        17-04-2021
 * @copyright   Copyright (c) 2021
 */

#pragma once

#include "Definitions.h"
#include "Point.h"

namespace space
{

/**
 * @class   Segment
 * @brief   The c++ representation of a segment.
 *          The segment is a part of a line that is bounded by two distinct endpoints
 *          and contains every point on the line between its endpoints.
 *
 * @details Class Segment: small class containing two points.
 *
 * @tparam TCrd The type of coordinates.
 */
template <typename TCrd>
class Segment : public std::pair<space::Point<TCrd>, space::Point<TCrd>>
{
public:

    /**
     * @brief   The type of coordinate.
     */
    using TCoordinate = TCrd;

public:

    constexpr Segment() = default;

    constexpr ~Segment() = default;

    constexpr Segment(Segment&&) noexcept = default;

    constexpr Segment(const Segment&) noexcept = default;

    constexpr Segment& operator=(Segment&&) noexcept = default;

    constexpr Segment& operator=(const Segment&) noexcept = default;

    constexpr std::strong_ordering operator<=>(const Segment<TCrd>&) const noexcept = default;

    /**
     * @brief           Initializes a new instance of the Segment structure that has the
     *                  specified first and the second point.
     *
     * @param first_    The first point.
     * @param second_   The second point.
     */
    constexpr Segment(const space::Point<TCrd>& first_, const space::Point<TCrd>& second_)
        : std::pair<space::Point<TCrd>, space::Point<TCrd>> {first_, second_}
    {
    }
}; // class Segment

} // namespace space
