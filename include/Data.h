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

#ifndef INCLUDE_DATA_H_
#define INCLUDE_DATA_H_

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "DatumBase.h"

namespace jino {
class Data {
 public:
  Data() = default;

  jino::DatumBase& operator[](const std::string&);
  const jino::DatumBase& operator[](const std::string&) const;

  template <typename T>
  void setValue(const std::string&, const T);

  template <typename T>
  T getValue(const std::string&) const;

  void forEachDatum(const std::function<void(const std::string&, const DatumBase&)>&) const;

  std::uint64_t size() const;

  std::uint8_t contains(const std::string&);

  void erase(const std::string&);
  void clear();

 private:
  std::map<const std::string, std::unique_ptr<DatumBase>> values_;
};
}  // namespace jino

#endif  // INCLUDE_DATA_H_
