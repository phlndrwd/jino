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

#include "OutputThread.h"

#include <chrono>
#include <filesystem>  /// NOLINT
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

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

jino::OutputThread::OutputThread() : date_(getFormattedDateStr()) {
  initOutDir();
}

void jino::OutputThread::initNetCDF() {
  threads_.enqueue(consts::eNetCDFThread, [this]() {
    netCDF_ = std::make_unique<NetCDFWriter>(date_);
  });
}

void jino::OutputThread::writeMetadata(const NetCDFData& netCDFData) {
  threads_.enqueue(consts::eNetCDFThread, [this, &netCDFData]() {
    getNetCDF().writeMetadata(netCDFData);
  });
}

void jino::OutputThread::writeDatums(const NetCDFData& netCDFData) {
  threads_.enqueue(consts::eNetCDFThread, [this, &netCDFData]() {
    getNetCDF().writeDatums(netCDFData);
  });
}

void jino::OutputThread::closeNetCDF() {
  threads_.enqueue(consts::eNetCDFThread, [this]() {
    getNetCDF().closeFile();
    netCDF_.reset();
  });
}

void jino::OutputThread::stop() {
  threads_.stopThreads();
}

const std::string& jino::OutputThread::getDate() const {
  return date_;
}

void jino::OutputThread::initOutDir() const {
  try {
    if (std::filesystem::exists(consts::kOutputDir) == false) {
      std::filesystem::create_directories(consts::kOutputDir);
    }
  } catch (const std::exception& error) {
    std::cerr << error.what() << std::endl;
  }
}

jino::NetCDFWriter& jino::OutputThread::getNetCDF() {
  if (netCDF_ == nullptr) {
    initNetCDF();
  }
  return *netCDF_;
}
