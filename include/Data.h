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

#ifndef INCLUDE_DATA_H_
#define INCLUDE_DATA_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "DatumBase.h"

namespace jino {
class Data {
 public:
  Data() = default;

  jino::DatumBase& operator[](const std::uint64_t);
  jino::DatumBase& operator[](const std::string&);

  const jino::DatumBase& operator[](const std::uint64_t) const;
  const jino::DatumBase& operator[](const std::string&) const;

  template <typename T>
  void setValue(const std::string&, const T);

  template <typename T>
  T getValue(const std::string&) const;

  const std::string& keyAt(const std::uint64_t) const;
  const std::vector<std::string>& keys() const;

  std::uint64_t size() const;

  std::uint8_t contains(const std::string&);

  void erase(const std::string&);
  void clear();

 private:
  std::vector<std::string> keys_;
  std::map<std::string, std::unique_ptr<jino::DatumBase>> values_;
};
}  // namespace jino

#endif  // INCLUDE_DATA_H_
