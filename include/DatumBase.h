/******************************************************************************
* JSON In NetCDF Out                                                          *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef INCLUDE_DATUMBASE_H_
#define INCLUDE_DATUMBASE_H_

#include <string>

namespace jino {
class DatumBase {
 public:
  DatumBase() {}
  virtual ~DatumBase() = default;

  virtual const std::string getValueStr() const = 0;
};
}  // namespace jino

#endif  // INCLUDE_DATUMBASE_H_
