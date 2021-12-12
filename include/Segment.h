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
 * @tparam TCrt The type of coordinates.
 */
template <typename TCrt>
class Segment : public std::pair<space::Point<TCrt>, space::Point<TCrt>>
{
public:

    /**
     * @brief   The type of coordinate.
     */
    using TCoordinate = TCrt;

public:

    constexpr Segment() = default;

    constexpr ~Segment() = default;

    constexpr Segment(Segment&&) noexcept = default;

    constexpr Segment(const Segment&) noexcept = default;

    constexpr Segment& operator=(Segment&&) noexcept = default;

    constexpr Segment& operator=(const Segment&) noexcept = default;

    constexpr std::strong_ordering operator<=>(const Segment<TCrt>&) const noexcept = default;

    /**
     * @brief           Initializes a new instance of the Segment structure that has the
     *                  specified first and the second point.
     *
     * @param first_    The first point.
     * @param second_   The second point.
     */
    constexpr Segment(const space::Point<TCrt>& first_, const space::Point<TCrt>& second_)
        : std::pair<space::Point<TCrt>, space::Point<TCrt>> {first_, second_}
    {
    }
}; // class Segment


namespace util
{

namespace impl
{

/**
 * @internal
 * @brief           Checks if point lies on line segment.
 *
 *
 * @tparam TCrt     The type of coordinates.
 * @param segment   The given segment.
 * @param point     The given point.
 * @return          true if the point lies on the line, otherwise false.
 */
template <typename TCrt>
constexpr bool onSegment(const Segment<TCrt>& segment, const Point <TCrt>& point) noexcept
{
    const auto[p, q] = segment;
    return ((point.x() <= std::max(p.x(), q.x()))
            && (point.x() >= std::min(p.x(), q.x()))
            && (point.y() <= std::max(p.y(), q.y()))
            && (point.y() >= std::min(p.y(), q.y())));
}

enum class EOrientation
{
    collinear = 0, clockwise, counterclockwise
};

/**
 * @internal
 * @brief       Finds orientation of ordered triplet (p, q, r).
 *
 * @tparam TCrt The type of coordinates.
 * @param p     The given point.
 * @param q     The given point.
 * @param r     The given point.
 * @return      The orientation (EOrientation::collinear, EOrientation::clockwise, EOrientation::counterclockwise)
 */
template <typename TCrt>
EOrientation orientation(const Point <TCrt>& p, const Point <TCrt>& q, const Point <TCrt>& r) noexcept
{
    const TCrt val = (q.y() - p.y()) * (r.x() - q.x())
                     - (q.x() - p.x()) * (r.y() - q.y());

    if (val == 0)
    {
        return EOrientation::collinear;
    }

    return (val > 0) ? EOrientation::clockwise : EOrientation::counterclockwise;
}

} // namespace util

/**
 * @brief           Returns true if the given segments has intersect
 *                  (i.e., there is at least one pixel that is within both segments),
 *                  otherwise returns false.
 *
 * @tparam TCrt     The type of coordinates.
 * @param first     The first segment.
 * @param second    The second segment.
 * @return          true if segments have an intersection, otherwise false.
 */
template <typename TCrt>
constexpr bool hasIntersect(const Segment<TCrt>& first, const Segment<TCrt>& second) noexcept
{
    const auto[p1, q1] = first;
    const auto[p2, q2] = second;

    // Find the four orientations needed for general and special cases
    const auto o1 = impl::orientation(p1, q1, p2);
    const auto o2 = impl::orientation(p1, q1, q2);
    const auto o3 = impl::orientation(p2, q2, p1);
    const auto o4 = impl::orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
    {
        return true;
    }

    // Special Cases
    // first and p2 are collinear and p2 lies on segment first
    if (o1 == impl::EOrientation::collinear && impl::onSegment(first, p2))
    {
        return true;
    }

    // first and q2 are collinear and q2 lies on segment first
    if (o2 == impl::EOrientation::collinear && impl::onSegment(first, q2))
    {
        return true;
    }

    // second and p1 are collinear and p1 lies on segment second
    if (o3 == impl::EOrientation::collinear && impl::onSegment(second, p1))
    {
        return true;
    }

    // second and q1 are collinear and q1 lies on segment second
    if (o4 == impl::EOrientation::collinear && impl::onSegment(second, q1))
    {
        return true;
    }

    // Doesn't fall in any of the above cases
    return false;
}
} // namespace util

} // namespace space
