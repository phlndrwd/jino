/**********************************************************************************************
* Jino (JSON In NetCDF Out).                                                                  *
*                                                                                             *
* (C) Copyright 2025, Phil Underwood. This file is part of Jino.                              *
*                                                                                             *
* Jino is free software: you can redistribute it and/or modify it under the terms of the GNU  *
* Lesser General Public License as published by the Free Software Foundation, either version  *
* 3 of the License, or (at your option) any later version.                                    *
*                                                                                             *
* Jino is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without   *
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the   *
* GNU Lesser General Public License for more details.                                         *
*                                                                                             *
* You should have received a copy of the GNU General Public License along with Jino. If not,  *
* see <https://www.gnu.org/licenses/>.                                                        *
**********************************************************************************************/

#ifndef INCLUDE_TYPES_H_
#define INCLUDE_TYPES_H_

#include <cstdint>
#include <string>

#include "Constants.h"

namespace jino {
template <typename T>
struct Types;

template <>
struct Types<std::int8_t> {
  static constexpr std::uint8_t type = consts::eInt8;
};

template <>
struct Types<std::int16_t> {
  static constexpr std::uint8_t type = consts::eInt16;
};

template <>
struct Types<std::int32_t> {
  static constexpr std::uint8_t type = consts::eInt32;
};

template <>
struct Types<std::int64_t> {
  static constexpr std::uint8_t type = consts::eInt64;
};

template <>
struct Types<std::uint8_t> {
  static constexpr std::uint8_t type = consts::eUInt8;
};

template <>
struct Types<std::uint16_t> {
  static constexpr std::uint8_t type = consts::eUInt16;
};

template <>
struct Types<std::uint32_t> {
  static constexpr std::uint8_t type = consts::eUInt32;
};

template <>
struct Types<std::uint64_t> {
  static constexpr std::uint8_t type = consts::eUInt64;
};

template <>
struct Types<float> {
  static constexpr std::uint8_t type = consts::eFloat;
};

template <>
struct Types<double> {
  static constexpr std::uint8_t type = consts::eDouble;
};

template <>
struct Types<std::string> {
  static constexpr std::uint8_t type = consts::eString;
};
}  // namespace jino

#endif // INCLUDE_TYPES_H_
