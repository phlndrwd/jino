/******************************************************************************
* Buffered Output Thread (BOT)                                                *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef DATUM_H_
#define DATUM_H_

#include <string>

#include "DatumBase.h"

namespace bot {

template<class T>
class Datum : public DatumBase {
 public:
  explicit Datum(const T);

  virtual ~Datum() = default;

  Datum()                        = delete;
  Datum(Datum&&)                 = delete;
  Datum(const Datum&)            = delete;
  Datum& operator=(Datum&&)      = delete;
  Datum& operator=(const Datum&) = delete;

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

}  // namespace bot

#endif   // DATUM_H_
