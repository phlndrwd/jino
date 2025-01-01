/******************************************************************************
* JSON In NetCDF Out                                                          *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

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
