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

#ifndef INCLUDE_DATUMBASE_H_
#define INCLUDE_DATUMBASE_H_

#include <cstdint>
#include <string>

namespace jino {
class DatumBase {
 public:
  explicit DatumBase(const std::uint8_t);

  virtual ~DatumBase() = default;

  virtual const std::string getValueStr() const = 0;

  const std::uint8_t& getType() const;

 protected:
  const std::uint8_t type_;
};
}  // namespace jino

#endif  // INCLUDE_DATUMBASE_H_
