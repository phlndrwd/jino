/******************************************************************************
* Buffered Output Thread                                                      *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Parameters.h"

#include "Datum.h"

bot::Parameters::Parameters() {}

template <typename T>
void bot::Parameters::setValue(const std::string& name, const T value) {
  auto it = values_.find(name);
  if (it == values_.end()) {
    values_.insert({name, std::make_unique<Datum<T>>(value)});
  } else {
    throw std::out_of_range("Parameter \"" + name + "\" alredy exists.");
  }
}

template void bot::Parameters::setValue<std::int8_t>(const std::string& name, const std::int8_t value);
template void bot::Parameters::setValue<std::int16_t>(const std::string& name, const std::int16_t value);
template void bot::Parameters::setValue<std::int32_t>(const std::string& name, const std::int32_t value);
template void bot::Parameters::setValue<std::int64_t>(const std::string& name, const std::int64_t value);
template void bot::Parameters::setValue<std::uint8_t>(const std::string& name, const std::uint8_t value);
template void bot::Parameters::setValue<std::uint16_t>(const std::string& name, const std::uint16_t value);
template void bot::Parameters::setValue<std::uint32_t>(const std::string& name, const std::uint32_t value);
template void bot::Parameters::setValue<std::uint64_t>(const std::string& name, const std::uint64_t value);
template void bot::Parameters::setValue<float>(const std::string& name, const float value);
template void bot::Parameters::setValue<double>(const std::string& name, const double value);
template void bot::Parameters::setValue<long double>(const std::string& name, const long double value);
template void bot::Parameters::setValue<std::string>(const std::string& name, const std::string value);

template <typename T>
T bot::Parameters::getValue(const std::string& name) const {
  auto it = values_.find(name);
  if (it != values_.end()) {
    bot::Datum<T>* datum = dynamic_cast<bot::Datum<T>*>(it->second.get());
    if (datum) {
      return datum->getValue();
    } else {
      throw std::runtime_error("Type mismatch or invalid cast.");
    }
  } else {
    throw std::out_of_range("Parameter \"" + name + "\" not found.");
  }
}

template std::int8_t bot::Parameters::getValue<std::int8_t>(const std::string& key) const;
template std::int16_t bot::Parameters::getValue<std::int16_t>(const std::string& key) const;
template std::int32_t bot::Parameters::getValue<std::int32_t>(const std::string& key) const;
template std::int64_t bot::Parameters::getValue<std::int64_t>(const std::string& key) const;
template std::uint8_t bot::Parameters::getValue<std::uint8_t>(const std::string& key) const;
template std::uint16_t bot::Parameters::getValue<std::uint16_t>(const std::string& key) const;
template std::uint32_t bot::Parameters::getValue<std::uint32_t>(const std::string& key) const;
template std::uint64_t bot::Parameters::getValue<std::uint64_t>(const std::string& key) const;
template float bot::Parameters::getValue<float>(const std::string& key) const;
template double bot::Parameters::getValue<double>(const std::string& key) const;
template long double bot::Parameters::getValue<long double>(const std::string& key) const;
template std::string bot::Parameters::getValue<std::string>(const std::string& key) const;

std::uint64_t bot::Parameters::size() {
  return values_.size();
}
