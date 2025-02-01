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
#include <memory>
#include <sstream>
#include <string>

#include "NetCDFThreadWriter.h"
#include "NetCDFWriter.h"
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

jino::Output::Output() : date_(getFormattedDateStr()) {
  initOutDir();
}

void jino::Output::initNetCDF(const std::uint8_t isMultiThread) {
  if (isMultiThread == consts::eMultiThread) {
    netCDF_ = std::make_unique<NetCDFThreadWriter>(date_);
  } else if (isMultiThread == consts::eSingleThread) {
    netCDF_ = std::make_unique<NetCDFWriter>(date_);
  } else {
    throw std::runtime_error("Unknown writer thread configuration...");
  }
}

const std::string& jino::Output::getDate() const {
  return date_;
}

jino::NetCDFWriterBase& jino::Output::getNetCDF() {
  if (netCDF_ == nullptr) {
    initNetCDF();  // Defaults to multi-threaded writer
  }
  return *netCDF_;
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
