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

#ifndef INCLUDE_BUFFERBASE_H_
#define INCLUDE_BUFFERBASE_H_

#include <cstdint>
#include <string>

namespace jino {
class BufferBase {
 public:
  explicit BufferBase(const std::string&);

  virtual ~BufferBase() = default;

  BufferBase() = delete;

  const std::string& getName() const;

  virtual std::uint64_t size() const = 0;
  virtual void print() = 0;

 private:
  const std::string name_;
};
}  // namespace jino

#endif  // INCLUDE_BUFFERBASE_H_
