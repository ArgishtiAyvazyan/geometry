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
 * @tparam  TCrt The type of coordinates.
 */
template <typename TCrt>
class Rect
{
public:
    using TCoordinate = TCrt;

private:
    using TPoint = space::Point<TCoordinate>;
public:

    constexpr Rect() = default;

    constexpr ~Rect() = default;

    constexpr Rect(Rect&&) noexcept = default;

    constexpr Rect(const Rect&) noexcept = default;

    constexpr Rect& operator=(Rect&&) noexcept = default;

    constexpr Rect& operator=(const Rect&) noexcept = default;

    constexpr std::strong_ordering operator<=>(const Rect<TCrt>&) const noexcept = default;

    /**
     * @brief   Initializes a new instance of the Rect structure that has the specified
     *          bottom-left corner location and the specified width and height.
     *
     * @param   pos A point that specifies the location of the bottom-left corner of the rectangle.
     * @param   width A value greater than or equal to 0 that specifies the Width of the Size structure.
     * @param   height A value greater than or equal to 0 that specifies the Height of the Size structure.
     */
    constexpr Rect(const TPoint& pos, TCoordinate width, TCoordinate height) noexcept
        : m_pos {pos}, m_width {width}, m_height {height}
    {
    }

    /**
     * @brief   Initializes a new instance of the Rect structure that has the specified
     *          bottom-left corner location and the specified top-right corner location.
     *
     * @param leftBottom    A point that specifies the location of the bottom-left corner of the rectangle.
     * @param rightTop      A point that specifies the location of the top-right corner of the rectangle.
     */
    constexpr Rect(const TPoint& leftBottom, const TPoint& rightTop) noexcept
        : m_pos {leftBottom}, m_width {rightTop.x() - leftBottom.x()}, m_height {rightTop.y() - leftBottom.y()}
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
 * @tparam  TCrt The type of coordinates.
 * @param   os The ostream.
 * @param   point The Rect.
 * @return  The reference to ostream.
 */
template <typename TOstream, typename TCrt>
TOstream& operator<<(TOstream& os, const space::Rect<TCrt>& rect)
{
    os << "Rect { { " << rect.pos().x() << ", " << rect.pos().y()
       << " }, " << rect.width() << ", " << rect.height() << " }";
    return os;
}


namespace util
{
/**
 * @brief Returns the position of the given rectangle top-left corner.
 *
 * @tparam TCrt The type of coordinates.
 * @param rect The rectangle.
 * @return top-left position.
 */
template <typename TCrt>
[[nodiscard]]
constexpr space::Point<TCrt> topLeftOf(const Rect<TCrt>& rect) noexcept
{
    return {rect.pos().x(), rect.pos.y() + rect.height()};
}

/**
 * @brief Returns the position of the given rectangle top-right corner.
 *
 * @tparam TCrt The type of coordinates.
 * @param rect The rectangle.
 * @return top-right position.
 */
template <typename TCrt>
[[nodiscard]]
constexpr space::Point<TCrt> topRightOf(const Rect<TCrt>& rect) noexcept
{
    return {rect.pos().x() + rect.width(), rect.pos().y() + rect.height()};
}

/**
 * @brief Returns the position of the given rectangle bottom-left corner.
 *
 * @tparam TCrt The type of coordinates.
 * @param rect The rectangle.
 * @return bottom-left position.
 */
template <typename TCrt>
[[nodiscard]]
constexpr space::Point<TCrt> bottomLeftOf(const Rect<TCrt>& rect) noexcept
{
    return rect.pos();
}

/**
 * @brief Returns the position of the given rectangle bottom-right corner.
 *
 * @tparam TCrt The type of coordinates.
 * @param rect The rectangle.
 * @return bottom-right position.
 */
template <typename TCrt>
[[nodiscard]]
constexpr space::Point<TCrt> bottomRightOf(const Rect<TCrt>& rect) noexcept
{
    return {rect.pos().x() + rect.width(), rect.pos().y()};
}

} // namespace util
} // namespace space

namespace std
{
template <typename TCrt>
struct tuple_size<::space::Rect<TCrt>>
    : integral_constant<size_t, 4>
{
};
} // namespace std

namespace std
{
template <size_t Index, typename TCrt>
struct tuple_element<Index, ::space::Rect<TCrt>>
{
    using type = TCrt;
    static_assert(Index < tuple_size_v<::space::Rect<TCrt>>, "Index out of bounds for Rect.");
};
} // namespace std

