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

#ifndef INCLUDE_DATUM_H_
#define INCLUDE_DATUM_H_

#include <string>

#include "DatumBase.h"

namespace jino {
template<class T>
class Datum : public DatumBase {
 public:
  explicit Datum(const T);
  virtual ~Datum() = default;

  Datum() = delete;

  const T getValue() const {
    return value_;
  }

  T getValue() {
    return value_;
  }

  const std::string getValueStr() const;

  void setValue(const T value) {
    value_ = value;
  }

 private:
  T value_;
};
}  // namespace jino

#endif   // INCLUDE_DATUM_H_
