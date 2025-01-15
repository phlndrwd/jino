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

#include "NetCDFData.h"

#include <string>
#include <vector>

void jino::NetCDFData::addDateToData(Data* const data, const std::string& formattedDate) {
  if (data != nullptr) {
    if (data->contains(consts::kDateKey) == true) {
      if (data->getValue<std::uint8_t>(consts::kDateKey) == true) {
        data->erase(consts::kDateKey);
        data->setValue<std::string>(consts::kDateKey, formattedDate);
      }
    }
    data_.push_back(data);
  }
}

void jino::NetCDFData::addData(Data* const data) {
  if (data != nullptr) {
    data_.push_back(data);
  }
}

void jino::NetCDFData::addDimension(const std::string& name, const std::uint64_t size,
                                    const std::uint8_t isUnlimited) {
  dimensions_.insert({size, {name, isUnlimited}});
}

void jino::NetCDFData::addDimension(const char* name, const std::uint64_t size,
                                    const std::uint8_t isUnlimited) {
  addDimension(std::string(name), size, isUnlimited);
}

std::string jino::NetCDFData::getDimensionName(const std::uint64_t size) const {
  return dimensions_.at(size).name;
}

const std::vector<jino::Data*>& jino::NetCDFData::getData() const {
  return data_;
}

void jino::NetCDFData::forEachDimension(const std::function<void(const Dimension&,
                                        const std::uint64_t)>& callback) const {
  for (const auto& [size, dim] : dimensions_) {
    callback(dim, size);
  }
}
