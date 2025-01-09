/**********************************************************************************************
* JSON In NetCDF Out                                                                          *
*                                                                                             *
* (C) Copyright 2025, Phil Underwood.                                                         *
*                                                                                             *
* This program is free software: you can redistribute it and/or modify it under the terms of  *
* the GNU General Public License as published by the Free Software Foundation, either version *
* 3 of the License, or (at your option) any later version.                                    *
*                                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;   *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   *
* See the GNU General Public License for more details.                                        *
*                                                                                             *
* You should have received a copy of the GNU General Public License along with this program.  *
* If not, see <https://www.gnu.org/licenses/>.                                                *
**********************************************************************************************/

#ifndef INCLUDE_BUFFERTRAITS_H_
#define INCLUDE_BUFFERTRAITS_H_

#include <cstdint>
#include <string>

#include "Constants.h"

namespace jino {
template <typename T>
struct BufferTraits;

template <>
struct BufferTraits<std::int8_t> {
  static constexpr std::uint8_t type = consts::eInt8;
};

template <>
struct BufferTraits<std::int16_t> {
  static constexpr std::uint8_t type = consts::eInt16;
};

template <>
struct BufferTraits<std::int32_t> {
  static constexpr std::uint8_t type = consts::eInt32;
};

template <>
struct BufferTraits<std::int64_t> {
  static constexpr std::uint8_t type = consts::eInt64;
};

template <>
struct BufferTraits<std::uint8_t> {
  static constexpr std::uint8_t type = consts::eUInt8;
};

template <>
struct BufferTraits<std::uint16_t> {
  static constexpr std::uint8_t type = consts::eUInt16;
};

template <>
struct BufferTraits<std::uint32_t> {
  static constexpr std::uint8_t type = consts::eUInt32;
};

template <>
struct BufferTraits<std::uint64_t> {
  static constexpr std::uint8_t type = consts::eUInt64;
};

template <>
struct BufferTraits<float> {
  static constexpr std::uint8_t type = consts::eFloat;
};

template <>
struct BufferTraits<double> {
  static constexpr std::uint8_t type = consts::eDouble;
};

template <>
struct BufferTraits<long double> {
  static constexpr std::uint8_t type = consts::eLongDouble;
};

template <>
struct BufferTraits<std::string> {
  static constexpr std::uint8_t type = consts::eString;
};
}  // namespace jino

#endif // INCLUDE_BUFFERTRAITS_H
