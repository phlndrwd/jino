/******************************************************************************
* Buffered Output Thread (BOT)                                                *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <map>
#include <memory>
#include <string>

#include "Constants.h"
#include "DatumBase.h"

namespace bot {

class Parameters {
 public:
  Parameters();

  bot::DatumBase& operator[](std::uint64_t index) {
    return *values_[consts::kParamNames[index]].get();
  }

  bot::DatumBase& operator[](std::string& name) {
    return *values_[name].get();
  }

  const bot::DatumBase& operator[](const std::uint64_t index) const {
    if (index >= consts::kParamNames.size()) {
      throw std::out_of_range("Index out of range.");
    }
    return this->operator[](consts::kParamNames[index]);
  }

  const bot::DatumBase& operator[](const std::string& name) const {
    auto it = values_.find(name);
    if (it != values_.end()) {
      return *it->second.get();
    } else {
      throw std::out_of_range("Key not found in map.");
    }
  }

  void setValue(const std::string&, const std::int8_t);
  void setValue(const std::string&, const std::int16_t);
  void setValue(const std::string&, const std::int32_t);
  void setValue(const std::string&, const std::int64_t);
  void setValue(const std::string&, const std::uint8_t);
  void setValue(const std::string&, const std::uint16_t);
  void setValue(const std::string&, const std::uint32_t);
  void setValue(const std::string&, const std::uint64_t);
  void setValue(const std::string&, const float);
  void setValue(const std::string&, const double);
  void setValue(const std::string&, const std::string);

  template <typename T>
  T getValue(const std::string& key) const;

  std::uint64_t size();

 private:
  std::map<std::string, std::unique_ptr<bot::DatumBase>> values_;
};

}

#endif // PARAMETERS_H
