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

#include "Buffers.h"

#include <stdexcept>
#include <string>

#include "Constants.h"

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
  if (buffer->getName() != consts::kEmptyString) {
    auto it = buffers_.find(buffer->getName());
    if (it == buffers_.end()) {
      buffers_.insert({buffer->getName(), buffer});
    } else {
      throw std::out_of_range("Buffer \"" + buffer->getName() + "\" alredy exists.");
    }
  } else {
    throw std::runtime_error("Buffer cannot have an empty name.");
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

void jino::Buffers::forEachBuffer(const std::function<void(const std::string&,
                                  BufferBase* const)>& callback) const {
  for (const auto& [name, buffer] : buffers_) {
    callback(name, buffer);
  }
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
