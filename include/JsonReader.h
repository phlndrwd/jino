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

#ifndef INCLUDE_JSONREADER_H_
#define INCLUDE_JSONREADER_H_

#include <fstream>
#include <iostream>
#include <string>

#include "nlohmann/json.hpp"

#include "Constants.h"
#include "Data.h"

namespace jino {
class JsonReader {
 public:
  JsonReader() = default;

  void readText(const std::string&, std::string&);

  void readParams(jino::Data&);
  void readAttrs(jino::Data&);

  template <typename T>
  T readState() {
    const std::filesystem::path path(consts::kInputDir + consts::kStateFile);
    try {
      std::ifstream file(path);
      nlohmann::json j;
      if (file.is_open()) {
        file >> j;
        file.close();
      }
      return j.get<T>();
    } catch (const std::exception& error) {
      std::cout << "ERROR: Could not open file \"" << path << "\"..."<< std::endl;
      std::cerr << error.what() << std::endl;
    }
    return T{};  // Default return in case of error
  };

 private:
  template <typename T>
  void setValue(jino::Data&, const std::string&, const T&);
  void setValue(jino::Data&, const std::string&, const std::uint8_t, const nlohmann::json&);
};
}  // namespace jino

#endif  // INCLUDE_JSONREADER_H_
