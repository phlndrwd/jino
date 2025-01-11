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

#ifndef INCLUDE_BUFFER_H_
#define INCLUDE_BUFFER_H_

#include "BufferBase.h"

#include <cstdint>
#include <string>
#include <vector>

namespace jino {
class Buffers;

template<class T>
class Buffer : public BufferBase {
 public:
  explicit Buffer(const std::string&, const std::uint64_t, T&);
  explicit Buffer(const char*, const std::uint64_t, T&);

  ~Buffer();

  Buffer()                         = delete;
  Buffer(Buffer&&)                 = delete;
  Buffer(const Buffer&)            = delete;
  Buffer& operator=(Buffer&&)      = delete;
  Buffer& operator=(const Buffer&) = delete;

  void record() override;
  void print() override;

  std::uint64_t size() const override;

  T& at(const std::uint64_t);
  const T& at(const std::uint64_t) const;

  T& setNext();
  const T& getNext();

  const std::vector<T>& getData() const;


 private:
  T& var_;
  std::vector<T> data_;

  std::uint64_t readIndex_;
  std::uint64_t writeIndex_;
};
}  // namespace jino

#endif // INCLUDE_BUFFER_H_
