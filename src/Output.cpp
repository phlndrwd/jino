/**********************************************************************************************
* JSON In NetCDF Out                                                                          *
*                                                                                             *
* (C) Copyright 2025, Phil Underwood.                                                         *
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

#include "Output.h"

#include <chrono>
#include <filesystem>
#include <format>
#include <stdexcept>
#include <string>

namespace {
  std::string getFormattedDateStr() {
    auto now = std::chrono::system_clock::now();
    auto nowSeconds = floor<std::chrono::seconds>(now);
    return std::format(jino::consts::kDateFormat, nowSeconds);
  }
}

jino::Output::Output() : formattedDate_(getFormattedDateStr()), attrs_(nullptr) {
  initDir();
}

jino::Output& jino::Output::get() {
  static Output this_;
  return this_;
}

void jino::Output::initDir() {
}


void jino::Output::record() {
  for (const auto& [name, buffer] : buffers_) {
    buffer->record();
  }
}

void jino::Output::addAttrs(Data* const attrs) {
  attrs_ = attrs;
  if (attrs_ != nullptr) {
    if (attrs_->contains(consts::kDateKey) == true) {
      if (attrs_->getValue<std::uint8_t>(consts::kDateKey) == true) {
        attrs_->erase(consts::kDateKey);
        attrs_->setValue<std::string>(consts::kDateKey, formattedDate_);
      }
    }
  }
}

void jino::Output::addParams(Data* const params) {
  params_ = params;
}

void jino::Output::attach(BufferBase* const buffer) {
  auto it = buffers_.find(buffer->getName());
  if (it == buffers_.end()) {
    buffers_.insert({buffer->getName(), buffer});
  } else {
    throw std::out_of_range("Buffer \"" + buffer->getName() + "\" alredy exists.");
  }
}

void jino::Output::detach(BufferBase* const buffer) {
  auto it = buffers_.find(buffer->getName());
  if (it != buffers_.end()) {
    buffers_.erase(it);
  } else {
    throw std::out_of_range("Buffer \"" + buffer->getName() + "\" not found.");
  }
}

void jino::Output::addDimension(const std::string& name, const std::uint64_t size) {
  dimensions_.insert({size, name});
}

void jino::Output::addDimension(const char* name, const std::uint64_t size) {
  addDimension(std::string(name), size);
}

std::string jino::Output::getDimensionName(const std::uint64_t size) const {
  return dimensions_.at(size);
}

void jino::Output::forEachDimension(const std::function<void(const std::string&,
                                     const std::uint64_t)>& callback) const {
  for (const auto& [size, name] : dimensions_) {
    callback(name, size);
  }
}

void jino::Output::forEachBuffer(const std::function<void(const std::string&,
                                  BufferBase* const)>& callback) const {
  for (const auto& [name, buffer] : buffers_) {
    callback(name, buffer);
  }
}

void jino::Output::toFile() {
  const std::string path = consts::kOutputDir + formattedDate_ + ".nc";
  const std::filesystem::path dir = std::filesystem::path(path).parent_path();
  try {
    if (!std::filesystem::exists(dir)) {
      std::filesystem::create_directories(dir);
    }
    File file(path, netCDF::NcFile::replace);
    writer_.toFile(file, attrs_, params_);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

void jino::Output::print() {
  for (auto const& [name, buffer] : buffers_) {
    if (buffer != nullptr) {
      buffer->print();
    } else {
      throw std::runtime_error("Type mismatch or invalid cast.");
    }
  }
}
