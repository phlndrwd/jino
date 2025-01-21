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

#ifndef INCLUDE_BUFFERS_H_
#define INCLUDE_BUFFERS_H_

#include <functional>
#include <map>
#include <string>

#include "BufferBase.h"

namespace jino {
class Buffers {
 public:
  Buffers(Buffers&&)                 = delete;
  Buffers(const Buffers&)            = delete;
  Buffers& operator=(Buffers&&)      = delete;
  Buffers& operator=(const Buffers&) = delete;

  static Buffers& get();

  void record();

  void attach(BufferBase* const);
  void detach(BufferBase* const);

  void forEachBuffer(const std::function<void(const std::string&, BufferBase* const)>&) const;

  void print();

 private:
  Buffers() = default;
  ~Buffers() = default;

  std::map<const std::string, BufferBase* const> buffers_;
};

}  // namespace jino

#endif // INCLUDE_BUFFERS_H_
