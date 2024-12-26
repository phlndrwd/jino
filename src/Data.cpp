/******************************************************************************
* JSON In NetCDF Out                                                          *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Data.h"

#include <memory>
#include <string>

#include "Datum.h"

jino::Data::Data() {}

template <typename T>
void jino::Data::setValue(const std::string& name, const T value) {
  auto it = values_.find(name);
  if (it == values_.end()) {
    values_.insert({name, std::make_unique<Datum<T>>(value)});
  } else {
    throw std::out_of_range("Parameter \"" + name + "\" alredy exists.");
  }
}

template void jino::Data::setValue<std::int8_t>(const std::string&, const std::int8_t);
template void jino::Data::setValue<std::int16_t>(const std::string&, const std::int16_t);
template void jino::Data::setValue<std::int32_t>(const std::string&, const std::int32_t);
template void jino::Data::setValue<std::int64_t>(const std::string&, const std::int64_t);
template void jino::Data::setValue<std::uint8_t>(const std::string&, const std::uint8_t);
template void jino::Data::setValue<std::uint16_t>(const std::string&, const std::uint16_t);
template void jino::Data::setValue<std::uint32_t>(const std::string&, const std::uint32_t);
template void jino::Data::setValue<std::uint64_t>(const std::string&, const std::uint64_t);
template void jino::Data::setValue<float>(const std::string&, const float);
template void jino::Data::setValue<double>(const std::string&, const double);
template void jino::Data::setValue<long double>(const std::string&, const long double);
template void jino::Data::setValue<std::string>(const std::string&, const std::string);

template <typename T>
T jino::Data::getValue(const std::string& name) const {
  auto it = values_.find(name);
  if (it != values_.end()) {
    jino::Datum<T>* datum = dynamic_cast<jino::Datum<T>*>(it->second.get());
    if (datum) {
      return datum->getValue();
    } else {
      throw std::runtime_error("Type mismatch or invalid cast.");
    }
  } else {
    throw std::out_of_range("Parameter \"" + name + "\" not found.");
  }
}

template std::int8_t jino::Data::getValue<std::int8_t>(const std::string&) const;
template std::int16_t jino::Data::getValue<std::int16_t>(const std::string&) const;
template std::int32_t jino::Data::getValue<std::int32_t>(const std::string&) const;
template std::int64_t jino::Data::getValue<std::int64_t>(const std::string&) const;
template std::uint8_t jino::Data::getValue<std::uint8_t>(const std::string&) const;
template std::uint16_t jino::Data::getValue<std::uint16_t>(const std::string&) const;
template std::uint32_t jino::Data::getValue<std::uint32_t>(const std::string&) const;
template std::uint64_t jino::Data::getValue<std::uint64_t>(const std::string&) const;
template float jino::Data::getValue<float>(const std::string&) const;
template double jino::Data::getValue<double>(const std::string&) const;
template long double jino::Data::getValue<long double>(const std::string&) const;
template std::string jino::Data::getValue<std::string>(const std::string&) const;

std::vector<std::string> jino::Data::keys() {
  std::vector<std::string> keys;
  for (auto const& [key, val] : values_) {
    keys.push_back(key);
  }
  return keys;
}

std::uint64_t jino::Data::size() {
  return values_.size();
}
