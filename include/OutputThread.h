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

#ifndef INCLUDE_OUTPUTTHREAD_H_
#define INCLUDE_OUTPUTTHREAD_H_

#include <cstdint>
#include <fstream>
#include <memory>
#include <string>

#include "nlohmann/json.hpp"

#include "Constants.h"
#include "Data.h"
#include "NetCDFWriter.h"
#include "ThreadQueues.h"

namespace jino {
class OutputThread {
 public:
  OutputThread();

  const std::string& getDate() const;

  void initNetCDF();
  void writeMetadata(const NetCDFData&);
  void writeDatums(const NetCDFData&);
  void closeNetCDF();

  template <typename T>
  void writeState(const T& system) {
    //threads_.enqueue(consts::eJSONThread, [this, &system]() {
      nlohmann::json j = system;
      std::filesystem::path path(consts::kOutputDir + date_ + consts::kJSONExtension);
      try {
        std::uint32_t count = 1;
        while (std::filesystem::exists(path) == true) {
          path = consts::kOutputDir + date_ + "(" + std::to_string(count) + ")" + consts::kJSONExtension;
          ++count;
        }
        std::ofstream file(path);
        if (file.is_open()) {
          file << std::setprecision(std::numeric_limits<double>::digits10 + 1);
          file << j.dump(consts::kJsonIndentSize);  // Indented output
          file.close();
        }
      } catch (const std::exception& error) {
        std::cout << "ERROR: Could not open file \"" << path << "\"..."<< std::endl;
        std::cerr << error.what() << std::endl;
      }
    //});
    //threads_.stopThread(consts::eJSONThread);
  }

  void waitForCompletion();

 private:
  void initOutDir() const;
  NetCDFWriter& getNetCDF();

  const std::string date_;

  ThreadQueues threads_;
  std::unique_ptr<NetCDFWriter> netCDF_;
};
}  // namespace jino

#endif // INCLUDE_OUTPUTTHREAD_H_
