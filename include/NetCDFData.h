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

#ifndef INCLUDE_NETCDFDATA_H_
#define INCLUDE_NETCDFDATA_H_

#include <map>
#include <vector>

#include "BufferBase.h"
#include "Data.h"

namespace jino {

struct Dimension {
  std::string name;
  std::uint8_t isUnlimited;

  Dimension(const std::string& _name)
      : name(_name), isUnlimited(false) {}

  Dimension(const std::string& _name, std::uint8_t _isUnlimited)
      : name(_name), isUnlimited(_isUnlimited) {}
};

class NetCDFData {
 public:
  NetCDFData() = default;

  void addDateToData(Data* const, const std::string&);
  void addData(Data* const);

  void addDimension(const std::string&, const std::uint64_t, const std::uint8_t = false);
  void addDimension(const char*, const std::uint64_t, const std::uint8_t = false);

  std::string getDimensionName(const std::uint64_t) const;

  const std::vector<Data*>& getData() const;

  void forEachDimension(const std::function<void(const Dimension&, const std::uint64_t)>&) const;

 private:
  std::vector<Data*> data_;

  std::map<const std::string, BufferBase* const> buffers_;
  std::map<const std::uint64_t, const Dimension> dimensions_;
};
}  // namespace jino

#endif // INCLUDE_NETCDFDATA_H_
