/**
 * @file        Rect.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaring the Point class and utilities for this.
 * @date        19-02-2021
 * @copyright   Copyright (c) 2021
 */

#ifndef RTREE_POINT_H
#define RTREE_POINT_H

#include <cmath>
#include <cstddef>
#include <tuple>

namespace space
{

/**
 * @brief   Represents an ordered pair of x- and y-coordinates that
 *          defines a point in a two-dimensional plane.
 *
 * @tparam  TCrd The type of coordinates.
 */
template <typename TCrd>
class Point
{
    using TCoordinate = TCrd;
public:
    constexpr Point() = default;

    constexpr ~Point() = default;

    constexpr Point(Point&&) noexcept = default;

    constexpr Point(const Point&) noexcept = default;

    constexpr Point& operator=(Point&&) noexcept = default;

    constexpr Point& operator=(const Point&) noexcept = default;

    constexpr std::strong_ordering operator<=>(const Point<TCrd>&) const noexcept = default;

    /**
     * @brief   Initializes a new instance of the Point struct with the specified coordinates.
     *
     * @param   x The x-axis coordinate.
     * @param   y The x-axis coordinate.
     */
    constexpr Point(TCoordinate x, TCoordinate y)
        : m_x {x},
        m_y {y}
    {
    }


    /**
     * @brief   Gets the x-axis value of the point.
     *
     * @return  The x-axis value.
     */
    [[nodiscard]]
    constexpr auto x() const noexcept
    {
        return m_x;
    }

    /**
     * @brief   Gets the x-axis value of the point.
     *
     * @return  The x-axis value.
     */
    [[nodiscard]]
    constexpr auto& x() noexcept
    {
        return m_x;
    }

    /**
     * @brief   Sets the x-axis value of the point.
     *
     * @param   newY The new x-axis value.
     */
    constexpr void setX(const TCoordinate newX) noexcept
    {
        m_x = newX;
    }

    /**
     * @brief   Gets the y-axis value of the point.
     *
     * @return  The y-axis value.
     */
    [[nodiscard]]
    constexpr auto y() const noexcept
    {
        return m_y;
    }

    /**
     * @brief   Gets the y-axis value of the point.
     *
     * @return  The y-axis value.
     */
    [[nodiscard]]
    constexpr auto& y() noexcept
    {
        return m_y;
    }

    /**
     * @brief   Sets the y-axis value of the point.
     *
     * @return  The y-axis value.
     */

    /**
     * @brief   Sets the y-axis value of the point.
     *
     * @param   newY The new y -axis value.
     */
    constexpr void setY(const TCoordinate newY) noexcept
    {
        m_x = newY;
    }


private:
    /**
     * @brief The x-axis value.
     *
     */
    TCoordinate m_x {0};

    /**
     * @brief The y-axis value.
     *
     */
    TCoordinate m_y {0};
}; // class Point


/**
 * @brief   The ostream operators for working with streams.
 *
 * @note    Templates used to avoid include stream headers.
 *
 * @tparam  TOstream The type of ostream.
 * @tparam  TCrd The type of coordinates.
 * @param   os The ostream.
 * @param   point The point.
 * @return  The reference to ostream.
 */
template <typename TOstream, typename TCrd>
TOstream& operator<<(TOstream& os, const space::Point<TCrd>& point)
{
    os << "Point { " << point.x() << ", " << point.y() << " }";
    return os;
}

namespace util
{

/**
 * @brief   Moves this given Point by the specified amount.
 *
 * @tparam  TCrd The type of coordinates.
 * @param   point The point for moving.
 * @param   deltaX The amount to offset the x-coordinate.
 * @param   deltaY The amount to offset the y-coordinate.
 */
template <typename TCrd>
constexpr void move(Point<TCrd>& point, TCrd deltaX, TCrd deltaY) noexcept
{
    point.setX(point.x() + deltaX);
    point.setY(point.y() + deltaY);
}

/**
 * @brief   Compute the distance between two given points.
 *
 * @tparam  TCrd The type of coordinates.
 * @tparam  TRetVal The type of return value (by default TCrd)
 * @param   first The first point.
 * @param   second The second point.
 * @return  The computed distance.
 */
template <typename TCrd, typename TRetVal = TCrd>
[[nodiscard]]
constexpr auto distance(const Point<TCrd>& first, const Point<TCrd>& second) -> TRetVal
{
    const auto xDelta = first.x() - second.x();
    const auto yDelta = first.y() - second.y();
    return std::sqrt(xDelta * xDelta + yDelta * yDelta);
}


} // namespace util

/**
 * The helper functions for structure binding.
*/

template <std::size_t Index, typename T>
constexpr auto pointGetHelper(T&& p)
{
    static_assert(Index < 2, "Index out of bounds for Point.");
    if constexpr (Index == 0)
    {
        return std::forward<T>(p).x();
    } else if constexpr (Index == 1)
    {
        return std::forward<T>(p).y();
    }
}

template <std::size_t Index, typename TCrd>
constexpr auto get(space::Point<TCrd>& p)
{
    return pointGetHelper<Index>(p);
}

template <std::size_t Index, typename TCrd>
constexpr auto get(space::Point<TCrd> const& p)
{
    return pointGetHelper<Index>(p);
}

template <std::size_t Index, typename TCrd>
constexpr auto get(space::Point<TCrd>&& p)
{
    return pointGetHelper<Index>(std::move(p));
}
} // namespace space

namespace std
{
template <typename TCrd>
struct tuple_size<::space::Point<TCrd>>
    : integral_constant<size_t, 2>
{
};
} // namespace std

namespace std
{
template <size_t Index, typename TCrd>
struct tuple_element<Index, ::space::Point<TCrd>>
    : conditional<Index == 0, TCrd, TCrd>
{
    static_assert(Index < tuple_size_v < ::space::Point<TCrd>>, "Index out of bounds for Point.");
};
} // namespace std

#endif //RTREE_POINT_H
