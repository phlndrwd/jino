/******************************************************************************
* Buffered Output Thread (BOT)                                                *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Datum.h"

#include <cstdint>

#include "Constants.h"

template<> bot::Datum<std::int8_t>::Datum(const std::int8_t value):
           DatumBase(consts::eInt8), value_(value) {}
template<> bot::Datum<std::int16_t>::Datum(const std::int16_t value):
           DatumBase(consts::eInt16), value_(value) {}
template<> bot::Datum<std::int32_t>::Datum(const std::int32_t value):
           DatumBase(consts::eInt32), value_(value) {}
template<> bot::Datum<std::int64_t>::Datum(const std::int64_t value):
           DatumBase(consts::eInt64), value_(value) {}
template<> bot::Datum<std::uint8_t>::Datum(const std::uint8_t value):
           DatumBase(consts::eUInt8), value_(value) {}
template<> bot::Datum<std::uint16_t>::Datum(const std::uint16_t value):
           DatumBase(consts::eUInt16), value_(value) {}
template<> bot::Datum<std::uint32_t>::Datum(const std::uint32_t value):
           DatumBase(consts::eUInt32), value_(value) {}
template<> bot::Datum<std::uint64_t>::Datum(const std::uint64_t value):
           DatumBase(consts::eUInt64), value_(value) {}
template<> bot::Datum<float>::Datum(const float value):
           DatumBase(consts::eFloat), value_(value) {}
template<> bot::Datum<double>::Datum(const double value):
           DatumBase(consts::eDouble), value_(value) {}
template<> bot::Datum<std::string>::Datum(const std::string value):
           DatumBase(consts::eString), value_(value) {}

template<class T>
const std::string bot::Datum<T>::getValueStr() const {
  return std::to_string(value_);
}

template const std::string bot::Datum<std::int8_t>::getValueStr() const;
template const std::string bot::Datum<std::int16_t>::getValueStr() const;
template const std::string bot::Datum<std::int32_t>::getValueStr() const;
template const std::string bot::Datum<std::int64_t>::getValueStr() const;
template const std::string bot::Datum<std::uint8_t>::getValueStr() const;
template const std::string bot::Datum<std::uint16_t>::getValueStr() const;
template const std::string bot::Datum<std::uint32_t>::getValueStr() const;
template const std::string bot::Datum<std::uint64_t>::getValueStr() const;
template const std::string bot::Datum<float>::getValueStr() const;
template const std::string bot::Datum<double>::getValueStr() const;

template<>
const std::string bot::Datum<std::string>::getValueStr() const {
  return value_;
}
