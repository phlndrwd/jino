/******************************************************************************
* JSON In NetCDF Out                                                          *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef INCLUDE_DATUM_H_
#define INCLUDE_DATUM_H_

#include <string>

#include "DatumBase.h"

namespace jino {
template<class T>
class Datum : public DatumBase {
 public:
  explicit Datum(const T);
  virtual ~Datum() = default;

  Datum() = delete;

  const T getValue() const {
    return value_;
  }

  T getValue() {
    return value_;
  }

  const std::string getValueStr() const;

  void setValue(const T value) {
    value_ = value;
  }

 private:
  T value_;
};
}  // namespace jino

#endif   // INCLUDE_DATUM_H_
