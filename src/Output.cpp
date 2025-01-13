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

#include <stdexcept>
#include <string>

jino::Output& jino::Output::get() {
  static Output this_;
  return this_;
}

void jino::Output::record() {
  for (const auto& [name, buffer] : buffers_) {
    buffer->record();
  }
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

void jino::Output::forEachBuffer(const std::function<void(const std::string&,
                                  BufferBase* const)>& callback) const {
  for (const auto& [name, buffer] : buffers_) {
    callback(name, buffer);
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
