/**********************************************************************************************
* JSON In NetCDF Out                                                                          *
*                                                                                             *
* (C) Copyright 2025, Phil Underwood.                                                         *
*                                                                                             *
* This program is free software: you can redistribute it and/or modify it under the terms of  *
* the GNU General Public License as published by the Free Software Foundation, either version *
* 3 of the License, or (at your option) any later version.                                    *
*                                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;   *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   *
* See the GNU General Public License for more details.                                        *
*                                                                                             *
* You should have received a copy of the GNU General Public License along with this program.  *
* If not, see <https://www.gnu.org/licenses/>.                                                *
**********************************************************************************************/

#include "Buffer.h"
#include "Buffers.h"
#include "Constants.h"

#include <iostream>
#include <stdexcept>
#include <string>

template <class T>
jino::Buffer<T>::Buffer(const std::string& name, const std::uint64_t size, Buffers* const parent) :
                 BufferBase(name), parent_(parent), buffer_(size), readIndex_(0), writeIndex_(0) {
  parent_->attach(this);
}

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

template <class T>
jino::Buffer<T>::~Buffer() {
  parent_->detach(this);
  buffer_.clear();
}

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

template<class T> T& jino::Buffer<T>::setNext() {
  if (writeIndex_ >= buffer_.size()) {
    throw std::out_of_range("WriteIndex out of range.");
  }
  std::uint64_t i = writeIndex_;
  ++writeIndex_;
  return buffer_.at(i);
}

template<class T> const T& jino::Buffer<T>::getNext() {
  if (readIndex_ >= buffer_.size() && readIndex_ <= writeIndex_) {
    throw std::out_of_range("ReadIndex out of range.");
  }
  std::uint64_t i = readIndex_;
  ++readIndex_;
  return buffer_.at(i);
}

template<class T>
std::uint64_t jino::Buffer<T>::size() const {
  return buffer_.size();
}

template<class T>
void jino::Buffer<T>::print() {
  for (std::uint64_t i = 0; i < size(); ++i) {
    std::cout << getName() << consts::kSeparator << getNext() << std::endl;
  }
}
