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

#ifndef INCLUDE_NETCDFDIM_H_
#define INCLUDE_NETCDFDIM_H_

#include <cstdint>
#include <string>

namespace jino {
struct NetCDFDim {
  const std::string name;
  const std::uint8_t isUnlimited;

  NetCDFDim(const std::string& name) : name(name), isUnlimited(false) {}
  NetCDFDim(const std::string& name, std::uint8_t isUnlimited) :
                                     name(name), isUnlimited(isUnlimited) {}
};
}  // namespace jino

#endif // INCLUDE_NETCDFDIM_H_
