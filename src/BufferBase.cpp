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

#include "BufferBase.h"

#include <string>

jino::BufferBase::BufferBase(const std::string& name, const std::string& group,
                             const std::uint8_t type) :
                  name_(name), group_(group), type_(type) {}

jino::BufferBase::BufferBase(const std::string& name, const std::uint8_t type) :
                  name_(name), group_(""), type_(type) {}

const std::string& jino::BufferBase::getName() const {
  return name_;
}

const std::string& jino::BufferBase::getGroup() const {
  return group_;
}

const std::uint8_t& jino::BufferBase::getType() const {
  return type_;
}
