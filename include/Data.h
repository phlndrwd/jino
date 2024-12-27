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
#include <vector>

#include "DatumBase.h"

namespace jino {
class Data {
 public:
  Data();

  jino::DatumBase& operator[](const std::uint64_t);
  jino::DatumBase& operator[](const std::string&);

  const jino::DatumBase& operator[](const std::uint64_t) const;
  const jino::DatumBase& operator[](const std::string&) const;

  template <typename T>
  void setValue(const std::string&, const T);

  template <typename T>
  T getValue(const std::string& key) const;

  const std::vector<std::string>& keys() const;
  std::uint64_t size() const;

  void clear();

 private:
  std::vector<std::string> keys_;
  std::map<std::string, std::unique_ptr<jino::DatumBase>> values_;
};
}  // namespace jino

#endif  // SRC_DATA_H_
