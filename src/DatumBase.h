/******************************************************************************
* Buffered Output Thread (BOT)                                                *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef DATUMBASE_H_
#define DATUMBASE_H_

#include <cstdint>
#include <string>

namespace bot {

class DatumBase {
 public:
  explicit DatumBase(const std::int8_t type) : type_(type) {}
  virtual ~DatumBase() = default;

  DatumBase()                            = delete;
  DatumBase(DatumBase&&)                 = delete;
  DatumBase(const DatumBase&)            = delete;
  DatumBase& operator=(DatumBase&&)      = delete;
  DatumBase& operator=(const DatumBase&) = delete;

  virtual const std::string getValueStr() const = 0;

  std::int8_t getType() const {
    return type_;
  }

 protected:
  std::int8_t type_;
};

}  // namespace bot

#endif  // DATUMBASE_H_
