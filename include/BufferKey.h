/**********************************************************************************************
* Jino (JSON In NetCDF Out).                                                                  *
*                                                                                             *
* (C) Copyright 2025, Phil Underwood.                                                         *
*                                                                                             *
* Jino is free software: you can redistribute it and/or modify it under the terms of the GNU  *
* Lesser General Public License as published by the Free Software Foundation, either version  *
* 3 of the License, or (at your option) any later version.                                    *
*                                                                                             *
* Jino is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without   *
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the   *
* GNU Lesser General Public License for more details.                                         *
*                                                                                             *
* You should have received a copy of the GNU Lesser General Public License along with Jino.   *
* If not, see <https://www.gnu.org/licenses/>.                                                *
**********************************************************************************************/

#ifndef INCLUDE_BUFFERKEY_H_
#define INCLUDE_BUFFERKEY_H_

#include <cstdint>
#include <string>
#include <tuple>

namespace jino {
struct BufferKey {
  const std::string varName;
  const std::string groupName;

  BufferKey(const std::string& varName, const std::string& groupName) :
                                        varName(varName), groupName(groupName) {}

  std::uint8_t operator < (const BufferKey& other) const {
    return std::tie(varName, groupName) < std::tie(other.varName, other.groupName);
  }
};
}  // namespace jino

#endif // INCLUDE_BUFFERKEY_H_
