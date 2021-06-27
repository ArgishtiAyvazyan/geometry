/**
 * @file        Point.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaring the Point class and utilities for this.
 * @date        19-02-2021
 * @copyright   Copyright (c) 2021
 */

#pragma once

#include <cmath>
#include <cstddef>
#include <tuple>

namespace space
{

/**
 * @brief   Represents an ordered pair of x- and y-coordinates that
 *          defines a point in a two-dimensional plane.
 *
 * @tparam  TCrt The type of coordinates.
 */
template <typename TCrt>
class Point
{
public:
    using TCoordinate = TCrt;

    constexpr Point() = default;

    constexpr ~Point() = default;

    constexpr Point(Point&&) noexcept = default;

    constexpr Point(const Point&) noexcept = default;

    constexpr Point& operator=(Point&&) noexcept = default;

    constexpr Point& operator=(const Point&) noexcept = default;

    constexpr std::strong_ordering operator<=>(const Point<TCrt>&) const noexcept = default;

    /**
     * @brief   Initializes a new instance of the Point struct with the specified coordinates.
     *
     * @param   x The x-axis coordinate.
     * @param   y The x-axis coordinate.
     */
    constexpr Point(TCoordinate x, TCoordinate y) noexcept
        : m_x {x}
        , m_y {y}
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
        m_y = newY;
    }

    /**
     * The helper functions for structure binding.
    */

    template<std::size_t Index>
    constexpr auto&& get()       &  noexcept { return getHelper<Index>(*this); }

    template<std::size_t Index>
    constexpr auto&& get()       && noexcept { return getHelper<Index>(*this); }

    template<std::size_t Index>
    constexpr auto&& get() const &  noexcept { return getHelper<Index>(*this); }

    template<std::size_t Index>
    constexpr auto&& get() const && noexcept { return getHelper<Index>(*this); }

private:
    template<std::size_t Index, typename T>
    static constexpr auto&& getHelper(T&& t) noexcept
    {
        static_assert(Index < 2
          , "Index out of bounds for space::Point");
        if constexpr (Index == 0) return std::forward<T>(t).m_x;
        if constexpr (Index == 1) return std::forward<T>(t).m_y;
    }


private:
    /**
     * @brief The x-axis value.
     */
    TCoordinate m_x {0};

    /**
     * @brief The y-axis value.
     */
    TCoordinate m_y {0};
}; // class Point

namespace util
{
/**
 * @brief   Moves given Point by the specified amount.
 *
 * @tparam  TCrt The type of coordinates.
 * @param   point The point for moving.
 * @param   deltaX The amount to offset the x-coordinate.
 * @param   deltaY The amount to offset the y-coordinate.
 */
template <typename TCrt>
constexpr void move(Point<TCrt>& point, TCrt deltaX, TCrt deltaY) noexcept
{
    point.setX(point.x() + deltaX);
    point.setY(point.y() + deltaY);
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
 * @param   point The point.
 * @return  The reference to ostream.
 */
template <typename TOstream, typename TCrt>
TOstream& operator<<(TOstream& os, const space::Point<TCrt>& point)
{
    os << "Point { " << point.x() << ", " << point.y() << " }";
    return os;
}

} // namespace space

namespace std
{
/**
 * The helper functions for structure binding.
*/

template <typename TCrt>
struct tuple_size<::space::Point<TCrt>>
    : integral_constant<size_t, 2>
{
};
} // namespace std

namespace std
{
template <size_t Index, typename TCrt>
struct tuple_element<Index, ::space::Point<TCrt>>
    : conditional<Index == 0, TCrt, TCrt>
{
    static_assert(Index < tuple_size_v < ::space::Point<TCrt>>, "Index out of bounds for Point.");
};
} // namespace std

