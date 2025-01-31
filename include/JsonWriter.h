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

#ifndef JSONWRITER_H
#define JSONWRITER_H

#include <fstream>

#include "Constants.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace jino {
class JsonWriter {
 public:
  JsonWriter() = default;

  template <typename T>
  void toFile(const T& system, const std::string& filePath) const {
  json j = system;
  std::ofstream file(filePath);
  if (file.is_open()) {
    file << std::setprecision(std::numeric_limits<double>::digits10 + 1);
    file << j.dump(consts::kJsonIndentSize);  // Indented output
    file.close();
  }
}

 private:
};
}  // namespace jino

#endif // JSONWRITER_H
