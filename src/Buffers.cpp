/**********************************************************************************************
* JSON In NetCDF Out                                                                          *
*                                                                                             *
* (C) Copyright 2025, Phil Underwood. All rights reserved.                                    *
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

#include "Buffers.h"

#include <algorithm>
#include <string>

jino::Buffers& jino::Buffers::get() {
  static Buffers this_;
  return this_;
}

template<class T>
jino::Buffer<T> jino::Buffers::newBuffer(const std::uint64_t size) {
  return jino::Buffer<T>(size, this);
}

template jino::Buffer<std::int8_t> jino::Buffers::newBuffer(const std::uint64_t);
template jino::Buffer<std::int16_t> jino::Buffers::newBuffer(const std::uint64_t);
template jino::Buffer<std::int32_t> jino::Buffers::newBuffer(const std::uint64_t);
template jino::Buffer<std::int64_t> jino::Buffers::newBuffer(const std::uint64_t);
template jino::Buffer<std::uint8_t> jino::Buffers::newBuffer(const std::uint64_t);
template jino::Buffer<std::uint16_t> jino::Buffers::newBuffer(const std::uint64_t);
template jino::Buffer<std::uint32_t> jino::Buffers::newBuffer(const std::uint64_t);
template jino::Buffer<std::uint64_t> jino::Buffers::newBuffer(const std::uint64_t);
template jino::Buffer<float> jino::Buffers::newBuffer(const std::uint64_t);
template jino::Buffer<double> jino::Buffers::newBuffer(const std::uint64_t);
template jino::Buffer<long double> jino::Buffers::newBuffer(const std::uint64_t);
template jino::Buffer<std::string> jino::Buffers::newBuffer(const std::uint64_t);

void jino::Buffers::attach(BufferBase* buffer) {
  buffers_.push_back(buffer);
}

void jino::Buffers::detach(BufferBase* buffer) {
  auto it = std::find(buffers_.begin(), buffers_.end(), buffer);
  if (it != buffers_.end()) {
    buffers_.erase(it);
  }
}
