/******************************************************************************
* JSON In, NetCDF Out                                                         *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Datum.h"

#include <cstdint>
#include <string>

template<> jino::Datum<std::int8_t>::Datum(const std::int8_t value): value_(value) {}
template<> jino::Datum<std::int16_t>::Datum(const std::int16_t value): value_(value) {}
template<> jino::Datum<std::int32_t>::Datum(const std::int32_t value): value_(value) {}
template<> jino::Datum<std::int64_t>::Datum(const std::int64_t value): value_(value) {}
template<> jino::Datum<std::uint8_t>::Datum(const std::uint8_t value): value_(value) {}
template<> jino::Datum<std::uint16_t>::Datum(const std::uint16_t value): value_(value) {}
template<> jino::Datum<std::uint32_t>::Datum(const std::uint32_t value): value_(value) {}
template<> jino::Datum<std::uint64_t>::Datum(const std::uint64_t value): value_(value) {}
template<> jino::Datum<float>::Datum(const float value): value_(value) {}
template<> jino::Datum<double>::Datum(const double value): value_(value) {}
template<> jino::Datum<long double>::Datum(const long double value): value_(value) {}
template<> jino::Datum<std::string>::Datum(const std::string value): value_(value) {}

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
