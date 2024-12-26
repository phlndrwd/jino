/******************************************************************************
* JSON In NetCDF Out                                                          *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef SRC_DATA_H_
#define SRC_DATA_H_

#include <map>
#include <memory>
#include <string>

#include "Constants.h"
#include "DatumBase.h"

namespace jino {
class Data {
 public:
  Data();

  jino::DatumBase& operator[](const std::uint64_t index) {
    if (index >= consts::kParamNames.size()) {
      throw std::out_of_range("Index out of range.");
    }
    return *values_[consts::kParamNames[index]].get();
  }

  jino::DatumBase& operator[](const std::string& name) {
    auto it = values_.find(name);
    if (it != values_.end()) {
      return *it->second.get();
    } else {
      throw std::out_of_range("Parameter \"" + name + "\" not found.");
    }
  }

  const jino::DatumBase& operator[](const std::uint64_t index) const {
    if (index >= consts::kParamNames.size()) {
      throw std::out_of_range("Index out of range.");
    }
    return this->operator[](consts::kParamNames[index]);
  }

  const jino::DatumBase& operator[](const std::string& name) const {
    auto it = values_.find(name);
    if (it != values_.end()) {
      return *it->second.get();
    } else {
      throw std::out_of_range("Parameter \"" + name + "\" not found.");
    }
  }

  template <typename T>
  void setValue(const std::string&, const T);

  template <typename T>
  T getValue(const std::string& key) const;

  std::uint64_t size();

 private:
  std::map<std::string, std::unique_ptr<jino::DatumBase>> values_;
};
}  // namespace jino

#endif  // SRC_DATA_H_
