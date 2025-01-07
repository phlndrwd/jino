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

#ifndef INCLUDE_BUFFERS_H_
#define INCLUDE_BUFFERS_H_

#include <functional>
#include <map>
#include <string>

#include "Buffer.h"
#include "BufferBase.h"
#include "Data.h"
#include "Writer.h"

namespace jino {
class Buffers {
 public:
  Buffers(Buffers&&)                 = delete;
  Buffers(const Buffers&)            = delete;
  Buffers& operator=(Buffers&&)      = delete;
  Buffers& operator=(const Buffers&) = delete;

  static Buffers& get();

  void attach(BufferBase* const);
  void detach(BufferBase* const);

  template<class T>
  Buffer<T> newBuffer(const std::string&, const std::uint64_t);

  template<class T>
  Buffer<T> newBuffer(const char*, const std::uint64_t);

  void addDimension(const std::string&, const std::uint64_t);
  void addDimension(const char*, const std::uint64_t);

  std::string getDimensionName(const std::uint64_t) const;

  void forEachDimension(const std::function<void(const std::string&, const std::uint64_t)>&) const;
  void forEachBuffer(const std::function<void(const std::string&, BufferBase* const)>&) const;

  void toFile(const Data&);
  void print();

 private:
  Buffers() = default;
  ~Buffers() = default;

  std::map<const std::string, BufferBase* const> buffers_;
  std::map<const std::uint64_t, const std::string> dimensions_;

  Writer writer_;
};

}  // namespace jino

#endif // INCLUDE_BUFFERS_H_
