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
#include "Constants.h"
#include "Buffers.h"
#include "Types.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

template <class T>
jino::Buffer<T>::Buffer(const std::string& name, const std::uint64_t size, const T& var) :
                 BufferBase(name, Types<T>::type), data_(size), var_(var),
                 readIndex_(0), writeIndex_(0) {
  Buffers::get().attach(this);
}

template <class T>
jino::Buffer<T>::Buffer(const char* name, const std::uint64_t size, const T& var) :
                 BufferBase(std::string(name), Types<T>::type), data_(size), var_(var),
                 readIndex_(0), writeIndex_(0) {
  Buffers::get().attach(this);
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
template class jino::Buffer<std::string>;

template <class T>
jino::Buffer<T>::~Buffer() {
  Buffers::get().detach(this);
  data_.clear();
}

template<class T> void jino::Buffer<T>::record() {
  if (writeIndex_ >= data_.size()) {
    throw std::out_of_range("WriteIndex out of range.");
  }
  data_.at(writeIndex_) = var_;
  ++writeIndex_;
}

template<class T>
void jino::Buffer<T>::print() {
  for (std::uint64_t i = 0; i < data_.size(); ++i) {
    std::cout << name_ << consts::kSeparator << data_.at(i) << std::endl;
  }
}

template<class T>
std::uint64_t jino::Buffer<T>::size() const {
  return data_.size();
}

template<class T>
std::uint64_t jino::Buffer<T>::getReadIndex() const {
  return readIndex_;
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

template<class T>
const std::vector<T>& jino::Buffer<T>::getData() const {
  return data_;
}
