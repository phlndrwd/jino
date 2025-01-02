/******************************************************************************
* JSON In NetCDF Out                                                          *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef INCLUDE_BUFFERBASE_H_
#define INCLUDE_BUFFERBASE_H_

#include <cstdint>

namespace jino {
class BufferBase {
 public:
  BufferBase() {}
  virtual ~BufferBase() = default;

  virtual std::uint64_t size() const = 0;
};
}  // namespace jino

#endif  // INCLUDE_BUFFERBASE_H_
