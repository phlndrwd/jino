/******************************************************************************
* Buffered Output Thread (BOT)                                                *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Parameters.h"

#include "Datum.h"

bot::Parameters::Parameters() {}

void bot::Parameters::setValue(const std::string& name, const std::int8_t value) {
  values_.insert({name, std::make_unique<Datum<std::int8_t>>(value)});
}

void bot::Parameters::setValue(const std::string& name, const std::int16_t value) {
  values_.insert({name, std::make_unique<Datum<std::int16_t>>(value)});
}

void bot::Parameters::setValue(const std::string& name, const std::int32_t value) {
  values_.insert({name, std::make_unique<Datum<std::int32_t>>(value)});
}

void bot::Parameters::setValue(const std::string& name, const std::int64_t value) {
  values_.insert({name, std::make_unique<Datum<std::int64_t>>(value)});
}

void bot::Parameters::setValue(const std::string& name, const std::uint8_t value) {
  values_.insert({name, std::make_unique<Datum<std::uint8_t>>(value)});
}

void bot::Parameters::setValue(const std::string& name, const std::uint16_t value) {
  values_.insert({name, std::make_unique<Datum<std::uint16_t>>(value)});
}

void bot::Parameters::setValue(const std::string& name, const std::uint32_t value) {
  values_.insert({name, std::make_unique<Datum<std::uint32_t>>(value)});
}

void bot::Parameters::setValue(const std::string& name, const std::uint64_t value) {
  values_.insert({name, std::make_unique<Datum<std::uint64_t>>(value)});
}

void bot::Parameters::setValue(const std::string& name, const float value) {
  values_.insert({name, std::make_unique<Datum<float>>(value)});
}

void bot::Parameters::setValue(const std::string& name, const double value) {
  values_.insert({name, std::make_unique<Datum<double>>(value)});
}

void bot::Parameters::setValue(const std::string& name, const std::string value) {
  values_.insert({name, std::make_unique<Datum<std::string>>(value)});
}

template <typename T>
T bot::Parameters::getValue(const std::string& key) const {
  auto it = values_.find(key);
  if (it != values_.end()) {
    bot::Datum<T>* datum = dynamic_cast<bot::Datum<T>*>(it->second.get());
    if (datum) {
      return datum->getValue();
    } else {
      throw std::runtime_error("Type mismatch or invalid cast.");
    }
  } else {
    throw std::out_of_range("Key not found in map.");
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
template std::string bot::Parameters::getValue<std::string>(const std::string& key) const;

std::uint64_t bot::Parameters::size() {
  return values_.size();
}
