/******************************************************************************
* Buffered Output Thread                                                      *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef DATUMBASE_H_
#define DATUMBASE_H_

#include <string>

namespace bot {

class DatumBase {
 public:
  DatumBase() {}
  virtual ~DatumBase() = default;

  DatumBase(DatumBase&&)                 = delete;
  DatumBase(const DatumBase&)            = delete;
  DatumBase& operator=(DatumBase&&)      = delete;
  DatumBase& operator=(const DatumBase&) = delete;

  virtual const std::string getValueStr() const = 0;
};

}  // namespace bot

#endif  // DATUMBASE_H_
