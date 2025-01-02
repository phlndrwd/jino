/***************************************************************************************************
* JSON In NetCDF Out                                                                               *
*                                                                                                  *
* (C) Copyright 2025, Phil Underwood. All rights reserved.                                         *
*                                                                                                  *
* This program is free software: you can redistribute it and/or modify it under the terms of the   *
* GNU General Public License as published by the Free Software Foundation, either version 3 of the *
* License, or (at your option) any later version.                                                  *
*                                                                                                  *
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;        *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See    *
* the GNU General Public License for more details.                                                 *
*                                                                                                  *
* You should have received a copy of the GNU General Public License along with this program. If    *
* not, see <https://www.gnu.org/licenses/>.                                                        *
***************************************************************************************************/

#ifndef INCLUDE_FILEREADER_H_
#define INCLUDE_FILEREADER_H_

#include <string>

#include "nlohmann/json.hpp"

#include "Data.h"
#include "File.h"

using json = nlohmann::json;

namespace jino {
class FileReader {
 public:
  FileReader();

  void read(std::string&, std::string&);
  void getParams(jino::Data&);

 private:
  template <typename T>
  void setParam(jino::Data&, const std::string&, const T&);
  void setParam(jino::Data&, const std::string&, const std::uint8_t, const nlohmann::json&);

  File file_;
};
}  // namespace jino

#endif  // INCLUDE_FILEREADER_H_
