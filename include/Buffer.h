/******************************************************************************
* JSON In NetCDF Out                                                          *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef INCLUDE_BUFFER_H_
#define INCLUDE_BUFFER_H_

#include "BufferBase.h"

#include <cstdint>
#include <vector>

namespace jino {
template<class T>
class Buffer : public BufferBase {
 public:
  explicit Buffer(const std::uint64_t);

  Buffer() = delete;

  T& at(const std::uint64_t);
  const T& at(const std::uint64_t) const;

  T& setNext();
  const T& getNext();

  std::uint64_t size() const override;

 private:
  std::vector<T> buffer_;

  std::uint64_t readIndex_;
  std::uint64_t writeIndex_;
};
}  // namespace jino

#endif // INCLUDE_BUFFER_H_
