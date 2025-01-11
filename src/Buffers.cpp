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

#include "Buffers.h"

#include <stdexcept>
#include <string>

jino::Buffers& jino::Buffers::get() {
  static Buffers this_;
  return this_;
}

void jino::Buffers::record() {
  for (const auto& [name, buffer] : buffers_) {
    buffer->record();
  }
}

void jino::Buffers::attach(BufferBase* const buffer) {
  auto it = buffers_.find(buffer->getName());
  if (it == buffers_.end()) {
    buffers_.insert({buffer->getName(), buffer});
  } else {
    throw std::out_of_range("Buffer \"" + buffer->getName() + "\" alredy exists.");
  }
}

void jino::Buffers::detach(BufferBase* const buffer) {
  auto it = buffers_.find(buffer->getName());
  if (it != buffers_.end()) {
    buffers_.erase(it);
  } else {
    throw std::out_of_range("Buffer \"" + buffer->getName() + "\" not found.");
  }
}

void jino::Buffers::addDimension(const std::string& name, const std::uint64_t size) {
  dimensions_.insert({size, name});
}

void jino::Buffers::addDimension(const char* name, const std::uint64_t size) {
  addDimension(std::string(name), size);
}

std::string jino::Buffers::getDimensionName(const std::uint64_t size) const {
  return dimensions_.at(size);
}

void jino::Buffers::forEachDimension(const std::function<void(const std::string&,
                                     const std::uint64_t)>& callback) const {
  for (const auto& [size, name] : dimensions_) {
    callback(name, size);
  }
}

void jino::Buffers::forEachBuffer(const std::function<void(const std::string&,
                                  BufferBase* const)>& callback) const {
  for (const auto& [name, buffer] : buffers_) {
    callback(name, buffer);
  }
}

void jino::Buffers::toFile(const jino::Data& attrs) {
  writer_.toFile(attrs);
}

void jino::Buffers::print() {
  for (auto const& [name, buffer] : buffers_) {
    if (buffer != nullptr) {
      buffer->print();
    } else {
      throw std::runtime_error("Type mismatch or invalid cast.");
    }
  }
}
