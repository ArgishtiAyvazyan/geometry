/**
 * @file        Rect.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaring the Rect class and utilities for this.
 * @date        20-02-2021
 * @copyright   Copyright (c) 2021
 */

#pragma once


#include <utility>
#include "Point.h"

namespace space
{

/**
 * @brief   Describes the width, height, and location of a rectangle.
 *
 *                  width
 *      ***************************** H
 *      *                           * e
 *      *                           * i
 *      *                           * g
 *      *                           * g
 *      ***************************** t
 *      Pos (x, y)
 *
 *
 * @tparam  TCrd The type of coordinates.
 */
template <typename TCrd>
class Rect
{
    using TCoordinate = TCrd;
    using TPoint = space::Point<TCoordinate>;
public:

    constexpr Rect() = default;

    constexpr ~Rect() = default;

    constexpr Rect(Rect&&) noexcept = default;

    constexpr Rect(const Rect&) noexcept = default;

    constexpr Rect& operator=(Rect&&) noexcept = default;

    constexpr Rect& operator=(const Rect&) noexcept = default;

    constexpr std::strong_ordering operator<=>(const Rect<TCrd>&) const noexcept = default;

    /**
     * @brief   Initializes a new instance of the Rect structure that has the specified
     *          bottom-left corner location and the specified width and height.
     *
     * @param   pos A point that specifies the location of the bottom-left corner of the rectangle.
     * @param   width A value greater than or equal to 0 that specifies the Width of the Size structure.
     * @param   height A value greater than or equal to 0 that specifies the Height of the Size structure.
     */
    constexpr Rect(const TPoint& pos, TCoordinate width, TCoordinate height)
        : m_pos {pos},
        m_width {width},
        m_height {height}
    {
    }

    /**
     * @brief   Gets the position of the bottom-left corner of the rectangle.
     *
     * @return  space::Point the bottom-left corner.
     */
    [[nodiscard]]
    constexpr auto pos() const noexcept -> const TPoint&
    {
        return m_pos;
    }

    /**
     * @brief   Gets the position of the bottom-left corner of the rectangle.
     *
     * @return  space::Point the bottom-left corner.
     */
    [[nodiscard]]
    constexpr auto pos() noexcept -> TPoint&
    {
        return m_pos;
    }

    /**
     * @brief   Sets the position of the bottom-left corner of the rectangle.
     *
     * @param   pos The new position.
     */
    constexpr void setPos(TPoint pos) noexcept
    {
        m_pos = std::move(pos);
    }

    /**
     * @brief   Gets the width of the rectangle.
     *
     * @return  The width.
     */
    [[nodiscard]]
    constexpr auto width() const noexcept
    {
        return m_width;
    }

    /**
     * @brief   Gets the width of the rectangle.
     *
     * @return  The reference to width.
     */
    [[nodiscard]]
    constexpr auto& width() noexcept
    {
        return m_width;
    }

    /**
     * @brief   Sets the width of the rectangle.
     *
     * @param   newWidth The new width.
     */
    constexpr void setWidth(const TCoordinate newWidth) noexcept
    {
        m_width = newWidth;
    }

    /**
     * @brief   Gets the height of the rectangle.
     *
     * @return  The height.
     */
    [[nodiscard]]
    constexpr auto height() const noexcept
    {
        return m_height;
    }

    /**
     * @brief   Gets the height of the rectangle.
     *
     * @return  The reference to height.
     */
    [[nodiscard]]
    constexpr auto& height() noexcept
    {
        return m_height;
    }

    /**
     * @brief   Sets the height of the rectangle.
     *
     * @param   newHeight The new height.
     */
    constexpr void setHeight(const TCoordinate newHeight) noexcept
    {
        m_height = newHeight;
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
        static_assert(Index < 4
          , "Index out of bounds for space::Point");
        if constexpr (Index == 0) return std::forward<T>(t).pos().x();
        if constexpr (Index == 1) return std::forward<T>(t).pos().y();
        if constexpr (Index == 2) return std::forward<T>(t).m_width;
        if constexpr (Index == 3) return std::forward<T>(t).m_height;
    }


private:

    /**
     * @brief The location of a rectangle.
     */
    TPoint m_pos {};

    /**
     * @brief The rectangle width.
     */
    TCoordinate m_width {};

    /**
     * @brief The rectangle height.
     */
    TCoordinate m_height {};
}; // class Rect

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
TOstream& operator<<(TOstream& os, const space::Rect<TCrd>& rect)
{
    os << "Rect { " << rect.pos().x() << ", " << rect.pos().y()
       << ", " << rect.width() << ", " << rect.height() << " }";
    return os;
}


namespace util
{

/**
 * @brief Moves the rectangle by the specified horizontal and vertical amounts.
 *
 * @tparam TCrd TCrd The type of coordinates.
 * @param rect he rectangle.
 * @param deltaX The horizontal amounts.
 * @param deltaY The vertical amounts.
 */
template <typename TCrd>
constexpr void move(Rect<TCrd>& rect, TCrd deltaX, TCrd deltaY) noexcept
{
    move(rect.pos(), deltaX, deltaY);
}

/**
 * @brief Returns the position of the given rectangle top-left corner.
 *
 * @tparam TCrd The type of coordinates.
 * @param rect The rectangle.
 * @return top-left position.
 */
template <typename TCrd>
[[nodiscard]]
constexpr auto topLeft(const Rect<TCrd>& rect) -> space::Point<TCrd>
{
    return {rect.pos().x(), rect.pos.y() + rect.height()};
}

/**
 * @brief Returns the position of the given rectangle top-right corner.
 *
 * @tparam TCrd The type of coordinates.
 * @param rect The rectangle.
 * @return top-right position.
 */
template <typename TCrd>
[[nodiscard]]
constexpr auto topRight(const Rect<TCrd>& rect) -> space::Point<TCrd>
{
    return {rect.pos().x() + rect.width(), rect.pos().y() + rect.height()};
}

/**
 * @brief Returns the position of the given rectangle bottom-left corner.
 *
 * @tparam TCrd The type of coordinates.
 * @param rect The rectangle.
 * @return bottom-left position.
 */
template <typename TCrd>
[[nodiscard]]
constexpr auto bottomLeft(const Rect<TCrd>& rect) -> space::Point<TCrd>
{
    return rect.pos();
}

/**
 * @brief Returns the position of the given rectangle bottom-right corner.
 *
 * @tparam TCrd The type of coordinates.
 * @param rect The rectangle.
 * @return bottom-right position.
 */
template <typename TCrd>
[[nodiscard]]
constexpr auto bottomRight(const Rect<TCrd>& rect) -> space::Point<TCrd>
{
    return {rect.pos().x() + rect.width(), rect.pos().y()};
}

/**
 * @brief Returns true if the given rectangles intersects
 *          (i.e., there is at least one pixel that is within both rectangles),
 *          otherwise returns false.
 *
 * @tparam TCrd The type of coordinates.
 * @param first The first rectangle.
 * @param second the second rectangle.
 * @return true if rectangles have an intersection, otherwise false.
 */
template <typename TCrd>
[[nodiscard]]
constexpr bool intersects(const Rect<TCrd>& first, const Rect<TCrd>& second)
{
    const auto[firstX1, firstY1] = bottomLeft(first);
    const auto[firstX2, firstY2] = topRight(first);
    const auto[secondX1, secondY1] = bottomLeft(second);
    const auto[secondX2, secondY2] = topRight(second);

    return firstX2 > secondX1 && secondX2 > firstX1
           && firstY2 > secondY1 && secondY2 > firstY1;
}


} // namespace util
} // namespace space

namespace std
{
template <typename TCrd>
struct tuple_size<::space::Rect<TCrd>>
    : integral_constant<size_t, 4>
{
};
} // namespace std

namespace std
{
template <size_t Index, typename TCrd>
struct tuple_element<Index, ::space::Rect<TCrd>>
{
    using type = TCrd;
    static_assert(Index < tuple_size_v<::space::Rect<TCrd>>, "Index out of bounds for Rect.");
};
} // namespace std

