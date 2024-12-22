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

std::uint64_t bot::Parameters::size() {
  return values_.size();
}
