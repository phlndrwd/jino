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
  for (const auto& [key, buffer] : buffers_) {
    buffer->record();
  }
}
void jino::Buffers::attach(BufferBase* const buffer) {
  if (buffer->getName() != consts::kEmptyString) {
    const std::string& groupName = buffer->getGroup();
    BufferKey key(buffer->getName(), groupName);
    auto it = buffers_.find(key);
    if (it == buffers_.end()) {
        buffers_.emplace(key, buffer);
    } else {
      if (groupName != consts::kEmptyString) {
        throw std::out_of_range("Buffer \"" + buffer->getName() +
                                "\" in group \"" + buffer->getGroup() + "\" already exists.");
      } else {
        throw std::out_of_range("Buffer \"" + buffer->getName() + "\" already exists.");
      }
    }
  } else {
      throw std::runtime_error("Buffer cannot have an empty name or group name.");
  }
}

void jino::Buffers::detach(BufferBase* const buffer) {
  const std::string& groupName = buffer->getGroup();
  BufferKey key(buffer->getName(), groupName);
  auto it = buffers_.find(key);
  if (it != buffers_.end()) {
    buffers_.erase(it);
  } else {
    throw std::out_of_range("Buffer \"" + buffer->getName() + "\" not found.");
    if (groupName != consts::kEmptyString) {
      throw std::out_of_range("Buffer \"" + buffer->getName() +
                              "\" in group \"" + buffer->getGroup() + "\" already exists.");
    } else {
      throw std::out_of_range("Buffer \"" + buffer->getName() + "\" not found.");
    }
  }
}

void jino::Buffers::forEachBuffer(const std::function<void(const BufferKey&,
                                  BufferBase* const)>& callback) const {
  for (const auto& [key, buffer] : buffers_) {
    callback(key, buffer);
  }
}

void jino::Buffers::print() {
  for (auto const& [key, buffer] : buffers_) {
    if (buffer != nullptr) {
      buffer->print();
    } else {
      throw std::runtime_error("Type mismatch or invalid cast.");
    }
  }
}
