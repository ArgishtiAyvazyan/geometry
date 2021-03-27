/**
 * @file        Utility.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaring the utilities for space.
 * @date        22-03-2021
 * @copyright   Copyright (c) 2021
 */


#pragma once

namespace space::util
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
 * @brief Moves the orthogonal shape by the specified horizontal and vertical amounts.
 *
 * @tparam TCrd The type of coordinates.
 * @param rect The orthogonal shape.
 * @param deltaX The horizontal amounts.
 * @param deltaY The vertical amounts.
 */
template <typename TOrthogonalShape, typename TCrd>
constexpr void move(TOrthogonalShape& rect, TCrd deltaX, TCrd deltaY) noexcept
{
    move(rect.pos(), deltaX, deltaY);
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
constexpr TRetVal distance(const Point<TCrd>& first, const Point<TCrd>& second) noexcept
{
    const auto xDelta = first.x() - second.x();
    const auto yDelta = first.y() - second.y();
    return std::sqrt(xDelta * xDelta + yDelta * yDelta);
}

/**
 * @brief Returns true if the given rectangles hesIntersects
 *          (i.e., there is at least one pixel that is within both rectangles),
 *          otherwise returns false.
 *
 *
 * @example
 *      hes intersection                                    hasn't intersection
 *
 *      *****************************                       *****************
 *      *                           *                       *               *
 *      *               *****************************       *               *
 *      *               *           *               *       *               *
 *      *               *           *               *       *****************
 *      *****************************               *
 *                      *                           *               *****************
 *                      *****************************               *               *
 *                                                                  *****************
 *
 *
 * @tparam TOrthogonalShape1 The type of first shape.
 * @tparam TOrthogonalShape2 The type of second shape.
 * @param first The first orthogonal shape.
 * @param second the second orthogonal shape.
 * @return true if rectangles have an intersection, otherwise false.
 */
 template <typename TOrthogonalShape1, typename TOrthogonalShape2>
[[nodiscard]]
constexpr bool hesIntersect(const TOrthogonalShape1& first, const TOrthogonalShape2& second) noexcept
{
    const auto[firstX1, firstY1] = bottomLeft(first);
    const auto[firstX2, firstY2] = topRight(first);
    const auto[secondX1, secondY1] = bottomLeft(second);
    const auto[secondX2, secondY2] = topRight(second);

    return firstX2 >= secondX1 && secondX2 >= firstX1 && firstY2 >= secondY1 && secondY2 >= firstY1;
}

/**
 * @brief  Returns true if the given point is inside or on the edge of the rectangle, otherwise returns false.
 *
 * @tparam  TOrthogonalShape The type of orthogonal object.
 * @tparam  TCrd The type of coordinates.
 * @param   rect The given rectangle.
 * @param   point The given point.
 * @return  true if contains, otherwise returns false.
 */
template <typename TOrthogonalShape, typename TCrd>
[[nodiscard]]
constexpr bool contains(const TOrthogonalShape& rect, const Point<TCrd>& point) noexcept
{
    const auto[x1, y1] = bottomLeft(rect);
    const auto[x2, y2] = topRight(rect);
    const auto[px, py] = point;
    return ((x1 <= px) && (px <= x2) && (y1 <= py) && (py <= y2));
}

/**
 * @brief   Returns true if the secondRect is inside the firstRect. otherwise returns false.
 *
 * @example
 *      contains                                            not contains
 *
 *      *****************************                       *****************
 *      *                           *                       *               *
 *      *               *********   *                       *               *
 *      *               *       *   *                       *               *
 *      *               *********   *                       *****************
 *      *****************************
 *                                                                  *****************
 *                                                                  *               *
 *                                                                  *****************
 * @tparam  TOrthogonalShape1 The type of first shape.
 * @tparam  TOrthogonalShape2 The type of second shape.
 * @param   firstRect The first rect.
 * @param   secondRect The second rect.
 * @return  true if contains, otherwise returns false.
 */
template <typename TOrthogonalShape1, typename TOrthogonalShape2>
[[nodiscard]]
constexpr bool contains(const TOrthogonalShape1& firstShape, const TOrthogonalShape2& secondShape) noexcept
{
    return space::util::contains(firstShape, secondShape.pos())
           && space::util::contains(firstShape, space::util::topRight(secondShape));
}


} // namespace util
