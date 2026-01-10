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

#include "Datum.h"

#include <cstdint>
#include <string>

#include "Types.h"

template<class T> jino::Datum<T>::Datum(const T value): DatumBase(Types<T>::type), value_(value) {}

template class jino::Datum<std::int8_t>;
template class jino::Datum<std::int16_t>;
template class jino::Datum<std::int32_t>;
template class jino::Datum<std::int64_t>;
template class jino::Datum<std::uint8_t>;
template class jino::Datum<std::uint16_t>;
template class jino::Datum<std::uint32_t>;
template class jino::Datum<std::uint64_t>;
template class jino::Datum<std::float32_t>;
template class jino::Datum<std::float64_t>;
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
template const std::string jino::Datum<std::float32_t>::getValueStr() const;
template const std::string jino::Datum<std::float64_t>::getValueStr() const;

template<>
const std::string jino::Datum<std::string>::getValueStr() const {
  return value_;
}

template<class T>
std::any jino::Datum<T>::getRawValue() const {
  return value_;
}

template std::any jino::Datum<std::int8_t>::getRawValue() const;
template std::any jino::Datum<std::int16_t>::getRawValue() const;
template std::any jino::Datum<std::int32_t>::getRawValue() const;
template std::any jino::Datum<std::int64_t>::getRawValue() const;
template std::any jino::Datum<std::uint8_t>::getRawValue() const;
template std::any jino::Datum<std::uint16_t>::getRawValue() const;
template std::any jino::Datum<std::uint32_t>::getRawValue() const;
template std::any jino::Datum<std::uint64_t>::getRawValue() const;
template std::any jino::Datum<std::float32_t>::getRawValue() const;
template std::any jino::Datum<std::float64_t>::getRawValue() const;
template std::any jino::Datum<std::string>::getRawValue() const;
