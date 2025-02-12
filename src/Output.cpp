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

#include "Output.h"

#include <chrono>
#include <filesystem>  /// NOLINT
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Constants.h"

namespace {
std::string getFormattedDateStr() {
  auto now = std::chrono::system_clock::now();
  auto nowSeconds = std::chrono::system_clock::to_time_t(now);

  std::ostringstream oss;
  std::string dateFormat = std::string(jino::consts::kDateFormat);
  oss << std::put_time(std::localtime(&nowSeconds), dateFormat.c_str());
  return oss.str();
}
}  // anonymous namespace

jino::Output::Output() : date_(getFormattedDateStr()), writer_(date_) {
  initOutDir();
}

void jino::Output::writeMetadata(const NetCDFData& netCDFData) {
  threads_.enqueue(consts::eNetCDFThread, [this, &netCDFData]() {
    writer_.writeMetadata(netCDFData);
  });
}

void jino::Output::writeDatums(const NetCDFData& netCDFData) {
  threads_.enqueue(consts::eNetCDFThread, [this, &netCDFData]() {
    writer_.writeDatums(netCDFData);
  });
}

void jino::Output::toFile(const NetCDFData& netCDFData) {
  threads_.enqueue(consts::eNetCDFThread, [this, &netCDFData]() {
    writer_.toFile(netCDFData);
  });
}

void jino::Output::closeNetCDF() {
  threads_.enqueue(consts::eNetCDFThread, [this]() {
    writer_.closeFile();
  });
}

void jino::Output::waitForCompletion() {
  threads_.stopThreads();
}

const std::string& jino::Output::getDate() const {
  return date_;
}

void jino::Output::initOutDir() const {
  try {
    if (std::filesystem::exists(consts::kOutputDir) == false) {
      std::filesystem::create_directories(consts::kOutputDir);
    }
  } catch (const std::exception& error) {
    std::cerr << error.what() << std::endl;
  }
}
