/**
 * @file        Definitions.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaring the definitions for geometry lib.
 * @date        03-04-2021
 * @copyright   Copyright (c) 2021
 */


#pragma once

#include <array>
#include <vector>
#include <stack>
#include <span>
#include <stdexcept>
#include <boost/container/flat_set.hpp>

namespace space
{
template <typename ... T>
using Vector = std::vector<T...>;

template <typename T, std::size_t N>
using Array = std::array<T, N>;

template <typename ... T>
using FlatSet = boost::container::flat_set<T...>;

template <typename ... T>
using Stack = std::stack<T...>;

template <typename ... T>
using Span = std::span<T...>;

} // namespace space
