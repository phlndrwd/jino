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
#include <vector>

template <class T>
jino::Buffer<T>::Buffer(const std::string& name, const std::uint64_t size, Buffers* const parent) :
                 BufferBase(name), parent_(parent), data_(size), readIndex_(0), writeIndex_(0) {
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
  data_.clear();
}

template<class T> T& jino::Buffer<T>::at(const std::uint64_t index) {
  if (index >= data_.size()) {
    throw std::out_of_range("Index out of range.");
  }
  return data_.at(index);
}

template<class T> const T& jino::Buffer<T>::at(const std::uint64_t index) const {
  if (index >= data_.size()) {
    throw std::out_of_range("Index out of range.");
  }
  return data_.at(index);
}

template<class T> T& jino::Buffer<T>::setNext() {
  if (writeIndex_ >= data_.size()) {
    throw std::out_of_range("WriteIndex out of range.");
  }
  std::uint64_t i = writeIndex_;
  ++writeIndex_;
  return data_.at(i);
}

template<class T> const T& jino::Buffer<T>::getNext() {
  if (readIndex_ >= data_.size() && readIndex_ <= writeIndex_) {
    throw std::out_of_range("ReadIndex out of range.");
  }
  std::uint64_t i = readIndex_;
  ++readIndex_;
  return data_.at(i);
}

template<> std::uint8_t jino::Buffer<std::int8_t>::getType() const {
  return consts::eInt8;
}

template<> std::uint8_t jino::Buffer<std::int16_t>::getType() const {
  return consts::eInt16;
}

template<> std::uint8_t jino::Buffer<std::int32_t>::getType() const {
  return consts::eInt32;
}

template<> std::uint8_t jino::Buffer<std::int64_t>::getType() const {
  return consts::eInt64;
}

template<> std::uint8_t jino::Buffer<std::uint8_t>::getType() const {
  return consts::eUInt8;
}

template<> std::uint8_t jino::Buffer<std::uint16_t>::getType() const {
  return consts::eUInt16;
}

template<> std::uint8_t jino::Buffer<std::uint32_t>::getType() const {
  return consts::eUInt32;
}

template<> std::uint8_t jino::Buffer<std::uint64_t>::getType() const {
  return consts::eUInt64;
}

template<> std::uint8_t jino::Buffer<float>::getType() const {
  return consts::eFloat;
}

template<> std::uint8_t jino::Buffer<double>::getType() const {
  return consts::eDouble;
}

template<> std::uint8_t jino::Buffer<long double>::getType() const {
  return consts::eLongDouble;
}

template<> std::uint8_t jino::Buffer<std::string>::getType() const {
  return consts::eString;
}

template<class T>
std::uint64_t jino::Buffer<T>::size() const {
  return data_.size();
}

template<class T>
const std::vector<T>& jino::Buffer<T>::getData() const {
  return data_;
}

template<class T>
void jino::Buffer<T>::print() {
  for (std::uint64_t i = 0; i < data_.size(); ++i) {
    std::cout << name_ << consts::kSeparator << data_.at(i) << std::endl;
  }
}
