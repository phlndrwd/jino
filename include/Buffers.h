/******************************************************************************
* JSON In NetCDF Out                                                          *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef INCLUDE_BUFFERS_H_
#define INCLUDE_BUFFERS_H_

#include "Buffer.h"
#include "BufferBase.h"

#include <map>
#include <memory>
#include <string>

namespace jino {
class Buffers {
 public:
  Buffers(Buffers&&)                 = delete;
  Buffers(const Buffers&)            = delete;
  Buffers& operator=(Buffers&&)      = delete;
  Buffers& operator=(const Buffers&) = delete;

  static Buffers& get();

  template<class T>
  Buffer<T> newBuffer(const std::string&, const std::uint64_t);

 private:
  Buffers() = default;
  ~Buffers() = default;

  std::map<std::string, std::unique_ptr<jino::BufferBase>> buffers_;
};

}  // namespace jino

#endif // INCLUDE_BUFFERS_H_
