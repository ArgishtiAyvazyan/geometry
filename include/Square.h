/**
 * @file        Square.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaring the Square class and utilities for this.
 * @date        19-03-2021
 * @copyright   Copyright (c) 2021
 */

#pragma once

namespace space
{

/**
 * @brief   Describes the size, and location of a Square.
 *
 *          Size
 *      *************
 *      *           *   S
 *      *           *   i
 *      *           *   z
 *      *           *   e
 *      *************
 *      Pos (x, y)
 *
 *
 * @tparam  TCrt The type of coordinates.
 */
template <typename TCrt>
class Square
{
public:
    using TCoordinate = TCrt;

private:
    using TPoint = space::Point<TCoordinate>;
public:

    constexpr Square() = default;

    constexpr ~Square() = default;

    constexpr Square(Square&&) noexcept = default;

    constexpr Square(const Square&) noexcept = default;

    constexpr Square& operator=(Square&&) noexcept = default;

    constexpr Square& operator=(const Square&) noexcept = default;

    constexpr std::strong_ordering operator<=>(const Square<TCrt>&) const noexcept = default;

    /**
     * @brief   Initializes a new instance of the Square structure that has the specified
     *          bottom-left corner location and the specified size.
     *
     * @param   pos A point that specifies the location of the bottom-left corner of the square.
     * @param   size A value greater than or equal to 0 that specifies the size of the structure.
     */
    constexpr Square(const TPoint& pos, TCoordinate width) noexcept
        : m_pos {pos}
        , m_size {width}
    {
    }

    /**
     * @brief   Gets the position of the bottom-left corner of the square.
     *
     * @return  space::Point the bottom-left corner.
     */
    [[nodiscard]]
    constexpr auto pos() const noexcept -> const TPoint&
    {
        return m_pos;
    }

    /**
     * @brief   Gets the position of the bottom-left corner of the square.
     *
     * @return  space::Point the bottom-left corner.
     */
    [[nodiscard]]
    constexpr auto pos() noexcept -> TPoint&
    {
        return m_pos;
    }

    /**
     * @brief   Sets the position of the bottom-left corner of the square.
     *
     * @param   pos The new position.
     */
    constexpr void setPos(TPoint pos) noexcept
    {
        m_pos = std::move(pos);
    }

    /**
     * @brief   Gets the size of the square.
     *
     * @return  The size.
     */
    [[nodiscard]]
    constexpr auto size() const noexcept
    {
        return m_size;
    }

    /**
     * @brief   Gets the size of the square.
     *
     * @return  The reference to size.
     */
    [[nodiscard]]
    constexpr auto& size() noexcept
    {
        return m_size;
    }

    /**
     * @brief   Sets the width of the square.
     *
     * @param   newWidth The new width.
     */
    constexpr void setSize(const TCoordinate newSize) noexcept
    {
        m_size = newSize;
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
        static_assert(Index < 3
          , "Index out of bounds for space::Point");
        if constexpr (Index == 0) return std::forward<T>(t).pos().x();
        if constexpr (Index == 1) return std::forward<T>(t).pos().y();
        if constexpr (Index == 2) return std::forward<T>(t).m_size;
    }


private:

    /**
     * @brief The location of a square.
     */
    TPoint m_pos {};

    /**
     * @brief The rectangle size.
     */
    TCoordinate m_size {};

}; // class Square

/**
 * @brief   The ostream operators for working with streams.
 *
 * @note    Templates used to avoid include stream headers.
 *
 * @tparam  TOstream The type of ostream.
 * @tparam  TCrt The type of coordinates.
 * @param   os The ostream.
 * @param   point The Square.
 * @return  The reference to ostream.
 */
template <typename TOstream, typename TCrt>
TOstream& operator<<(TOstream& os, const space::Square<TCrt>& rect)
{
    os << "Square { { " << rect.pos().x() << ", " << rect.pos().y()
       << " }, " << rect.size() << " }";
    return os;
}


namespace util
{

/**
 * @brief Returns the position of the given square top-left corner.
 *
 * @tparam TCrt The type of coordinates.
 * @param square The square.
 * @return top-left position.
 */
template <typename TCrt>
[[nodiscard]]
constexpr space::Point<TCrt> topLeftOf(const Square<TCrt>& square) noexcept
{
    return {square.pos().x(), square.pos.y() + square.size()};
}

/**
 * @brief Returns the position of the given square top-right corner.
 *
 * @tparam TCrt The type of coordinates.
 * @param square The square.
 * @return top-right position.
 */
template <typename TCrt>
[[nodiscard]]
constexpr space::Point<TCrt> topRightOf(const Square<TCrt>& square) noexcept
{
    return {square.pos().x() + square.size(), square.pos().y() + square.size()};
}

/**
 * @brief Returns the position of the given square bottom-left corner.
 *
 * @tparam TCrt The type of coordinates.
 * @param square The square.
 * @return bottom-left position.
 */
template <typename TCrt>
[[nodiscard]]
constexpr space::Point<TCrt> bottomLeftOf(const Square<TCrt>& square) noexcept
{
    return square.pos();
}

/**
 * @brief Returns the position of the given square bottom-right corner.
 *
 * @tparam TCrt The type of coordinates.
 * @param square The square.
 * @return bottom-right position.
 */
template <typename TCrt>
[[nodiscard]]
constexpr space::Point<TCrt> bottomRightOf(const Square<TCrt>& square) noexcept
{
    return {square.pos().x() + square.size(), square.pos().y()};
}
} // namespace util
} // namespace space

namespace std
{
template <typename TCrt>
struct tuple_size<::space::Square<TCrt>>
    : integral_constant<size_t, 4>
{
};
} // namespace std

namespace std
{
template <size_t Index, typename TCrt>
struct tuple_element<Index, ::space::Square<TCrt>>
{
    using type = TCrt;
    static_assert(Index < tuple_size_v<::space::Square<TCrt>>, "Index out of bounds for Square.");
};
} // namespace std
