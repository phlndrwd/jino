/***************************************************************************************************
* JSON In NetCDF Out                                                                               *
*                                                                                                  *
* (C) Copyright 2025, Phil Underwood. All rights reserved.                                         *
*                                                                                                  *
* This program is free software: you can redistribute it and/or modify it under the terms of the   *
* GNU General Public License as published by the Free Software Foundation, either version 3 of the *
* License, or (at your option) any later version.                                                  *
*                                                                                                  *
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;        *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See    *
* the GNU General Public License for more details.                                                 *
*                                                                                                  *
* You should have received a copy of the GNU General Public License along with this program. If    *
* not, see <https://www.gnu.org/licenses/>.                                                        *
***************************************************************************************************/

#include "Datum.h"

#include <cstdint>
#include <string>

template<class T> jino::Datum<T>::Datum(const T value): value_(value) {}

template class jino::Datum<std::int8_t>;
template class jino::Datum<std::int16_t>;
template class jino::Datum<std::int32_t>;
template class jino::Datum<std::int64_t>;
template class jino::Datum<std::uint8_t>;
template class jino::Datum<std::uint16_t>;
template class jino::Datum<std::uint32_t>;
template class jino::Datum<std::uint64_t>;
template class jino::Datum<float>;
template class jino::Datum<double>;
template class jino::Datum<long double>;
template class jino::Datum<std::string>;

template<class T>
const std::string jino::Datum<T>::getValueStr() const {
  return std::to_string(value_);
}

template const std::string jino::Datum<std::int8_t>::getValueStr() const;
template const std::string jino::Datum<std::int16_t>::getValueStr() const;
template const std::string jino::Datum<std::int32_t>::getValueStr() const;
template const std::string jino::Datum<std::int64_t>::getValueStr() const;
template const std::string jino::Datum<std::uint8_t>::getValueStr() const;
template const std::string jino::Datum<std::uint16_t>::getValueStr() const;
template const std::string jino::Datum<std::uint32_t>::getValueStr() const;
template const std::string jino::Datum<std::uint64_t>::getValueStr() const;
template const std::string jino::Datum<float>::getValueStr() const;
template const std::string jino::Datum<double>::getValueStr() const;
template const std::string jino::Datum<long double>::getValueStr() const;

template<>
const std::string jino::Datum<std::string>::getValueStr() const {
  return value_;
}
