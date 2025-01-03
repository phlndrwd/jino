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

#include "Buffer.h"
#include "BufferBase.h"

#include <string>
#include <vector>

namespace jino {
class Buffers {
 public:
  Buffers(Buffers&&)                 = delete;
  Buffers(const Buffers&)            = delete;
  Buffers& operator=(Buffers&&)      = delete;
  Buffers& operator=(const Buffers&) = delete;

  static Buffers& get();

  void attach(BufferBase*);
  void detach(BufferBase*);

  template<class T>
  Buffer<T> newBuffer(const std::string&, const std::uint64_t);

  template<class T>
  Buffer<T> newBuffer(const char*, const std::uint64_t);

  void print();

 private:
  Buffers() = default;
  ~Buffers() = default;

  std::vector<BufferBase*> buffers_;
};

}  // namespace jino

#endif // INCLUDE_BUFFERS_H_
