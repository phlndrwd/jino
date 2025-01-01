/******************************************************************************
* JSON In NetCDF Out                                                          *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Buffer.h"

#include <stdexcept>
#include <string>

template <class T>
jino::Buffer<T>::Buffer(const std::uint64_t size) : buffer_(size), readIndex_(0), writeIndex_(0) {}

template class jino::Buffer<std::int8_t>;
template class jino::Buffer<std::int16_t>;
template class jino::Buffer<std::int32_t>;
template class jino::Buffer<std::int64_t>;
template class jino::Buffer<std::uint8_t>;
template class jino::Buffer<std::uint16_t>;
template class jino::Buffer<std::uint32_t>;
template class jino::Buffer<std::uint64_t>;
template class jino::Buffer<float>;
template class jino::Buffer<double>;
template class jino::Buffer<long double>;
template class jino::Buffer<std::string>;

template<class T> T& jino::Buffer<T>::at(const std::uint64_t index) {
  if (index >= buffer_.size()) {
    throw std::out_of_range("Index out of range.");
  }
  return buffer_.at(index);
}

template<class T> const T& jino::Buffer<T>::at(const std::uint64_t index) const {
  if (index >= buffer_.size()) {
    throw std::out_of_range("Index out of range.");
  }
  return buffer_.at(index);
}

template<class T> T& jino::Buffer<T>::set() {
  if (writeIndex_ >= buffer_.size()) {
    throw std::out_of_range("WriteIndex out of range.");
  }
  std::uint64_t i = writeIndex_;
  ++writeIndex_;
  return buffer_.at(i);
}

template<class T> const T& jino::Buffer<T>::get() {
  if (readIndex_ >= buffer_.size() && readIndex_ <= writeIndex_) {
    throw std::out_of_range("ReadIndex out of range.");
  }
  std::uint64_t i = readIndex_;
  ++readIndex_;
  return buffer_.at(i);
}

template<class T>
std::uint64_t jino::Buffer<T>::size() {
  return buffer_.size();
}
