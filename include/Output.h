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

#ifndef INCLUDE_OUTPUT_H_
#define INCLUDE_OUTPUT_H_

#include <cstdint>
#include <fstream>
#include <memory>
#include <string>

#include "nlohmann/json.hpp"

#include "Constants.h"
#include "NetCDFWriterBase.h"

namespace jino {
class Output {
 public:
  Output();

  void initNetCDF(const std::uint8_t = consts::eMultiThread);

  const std::string& getDate() const;
  NetCDFWriterBase& getNetCDF();

  template <typename T>
  void writeState(const T& system) const {
    nlohmann::json j = system;
    const std::string outputPath = consts::kOutputDir + date_ + "_" + consts::kStateFile;
    try {
      std::ofstream file(outputPath);
      if (file.is_open()) {
        file << std::setprecision(std::numeric_limits<double>::digits10 + 1);
        file << j.dump(consts::kJsonIndentSize);  // Indented output
        file.close();
      }
    } catch (const std::exception& error) {
      std::cout << "ERROR: Could not open file \"" << outputPath << "\"..."<< std::endl;
      std::cerr << error.what() << std::endl;
    }
  }

 private:
  void initOutDir() const;

  const std::string date_;
  std::unique_ptr<NetCDFWriterBase> netCDF_;
};
}  // namespace jino

#endif // INCLUDE_OUTPUT_H_
