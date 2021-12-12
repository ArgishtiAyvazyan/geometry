/**
 * @file    Vector.h
 * @author  Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief   Declaring the Vector class.
 * @date    11-12-2021
 * @copyright Copyright (c) 2021
 */

#include "Point.h"

namespace space
{

/**
 * @brief Represents a geometric vector in a two-dimensional plane.
 *
 * @tparam  TCrt The type of coordinates.
 */
template <typename TCrt>
class Vector
{
public:
    using TCoordinate = TCrt;

    constexpr Vector() = default;

    constexpr ~Vector() = default;

    constexpr Vector(Vector&&) noexcept = default;

    constexpr Vector(const Vector&) noexcept = default;

    constexpr Vector& operator=(Vector&&) noexcept = default;

    constexpr Vector& operator=(const Vector&) noexcept = default;

    constexpr std::strong_ordering operator<=>(const Vector<TCrt>&) const noexcept = default;

    /**
     * @brief Initializes a new instance of the Vector struct with the specified point.
     *
     * @param top The top coordinate.
     */
    constexpr Vector(const space::Point<TCrt>& top)
        : m_top { top }
    {
    }

    /**
     * @brief   Initializes a new instance of the Vector struct with the specified coordinates.
     *
     * @param   x The x-axis coordinate.
     * @param   y The x-axis coordinate.
     */
    constexpr Vector(TCoordinate x, TCoordinate y) noexcept
        : m_top {x, y}
    {
    }

    /**
     * @brief   Returns the top coordinate.
     *
     * @return  space::Point<TCrt> The top coordinate.
     */
    constexpr auto& top() const
    {
        return m_top;
    }

    /**
     * @brief Set the Top coordinate.
     *
     * @param newTop The new top coordinate.
     */
    void setTop(const space::Point<TCrt>& newTop)
    {
        m_top = std::move(newTop);
    }

private:
    space::Point<TCrt> m_top;
}; // class Vector

template <typename TCrt>
[[nodiscard]]
constexpr Vector<TCrt> operator+(const Vector<TCrt>& lhs, const Vector<TCrt>& rhs) noexcept
{
    return { lhs.top().x() + rhs.top().x(), lhs.top().y() + rhs.top().y() };
}

template <typename TCrt>
[[nodiscard]]
constexpr Vector<TCrt> operator-(const Vector<TCrt>& lhs, const Vector<TCrt>& rhs) noexcept
{
    return { lhs.top().x() - rhs.top().x(), lhs.top().y() - rhs.top().y() };
}

namespace util
{

template <typename TCrt>
[[nodiscard]]
constexpr TCrt dotProduct(const Vector<TCrt>& first, const Vector<TCrt>& second) noexcept
{
    return first.top().x() * second.top().x() + first.top().y() * second.top().y();
}

/**
 * @brief           Returns the absolute value for a given vector.
 *
 * @tparam TCrt     TCrt The type of coordinates.
 * @param vector    The vector.
 * @return TCrt     The absolute value.
 */
template <typename TCrt>
[[nodiscard]]
constexpr TCrt absoluteValue(const Vector<TCrt>& vector) noexcept
{
    return std::sqrt(vector.top().x() * vector.top().x() + vector.top().y() * vector.top().y());
}

/**
 * @brief               Returns normalized vector for a given vector.
 *
 * @tparam TCrt         TCrt The type of coordinates.
 * @param vector        The source vector.
 * @return Vector<TCrt> The normalized vector.
 */
template <typename TCrt>
[[nodiscard]]
constexpr Vector<TCrt> normalize(const Vector<TCrt>& vector) noexcept
{
    auto magnitude = absoluteValue(vector);
    if (magnitude == 0)
    {
        return { 0, 0 };
    }
    auto top = vector.top();
    top.x() /= magnitude;
    top.y() /= magnitude;
    return { top };
}

/**
 * @brief               Returns normalized axis perpendicular for a given vector.
 *
 * @tparam TCrt         TCrt The type of coordinates.
 * @param vector        The source vector.
 * @return Vector<TCrt> The normalized axis perpendicular.
 */
template <typename TCrt>
[[nodiscard]]
constexpr Vector<TCrt> axisPerpendicularOf(const Vector<TCrt>& vector)
{
    Vector axis {-vector.top().y(), vector.top().x()};
    return util::normalize(axis);
}

} // namespace utils

} // namespace space

