/**********************************************************************************************
* Jino (JSON In NetCDF Out).                                                                  *
*                                                                                             *
* (C) Copyright 2025, Phil Underwood.                                                         *
*                                                                                             *
* Jino is free software: you can redistribute it and/or modify it under the terms of the GNU  *
* Lesser General Public License as published by the Free Software Foundation, either version  *
* 3 of the License, or (at your option) any later version.                                    *
*                                                                                             *
* Jino is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without   *
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the   *
* GNU Lesser General Public License for more details.                                         *
*                                                                                             *
* You should have received a copy of the GNU Lesser General Public License along with Jino.   *
* If not, see <https://www.gnu.org/licenses/>.                                                *
**********************************************************************************************/

#include "Data.h"

#include <memory>
#include <stdexcept>
#include <string>

#include "Datum.h"

jino::DatumBase& jino::Data::operator[](const std::string& key) {
  auto it = values_.find(key);
  if (it != values_.end()) {
    return *it->second.get();
  } else {
    throw std::out_of_range("Datum \"" + key + "\" not found.");
  }
}

const jino::DatumBase& jino::Data::operator[](const std::string& key) const {
  auto it = values_.find(key);
  if (it != values_.end()) {
    return *it->second.get();
  } else {
    throw std::out_of_range("Datum \"" + key + "\" not found.");
  }
}

template <typename T>
void jino::Data::setValue(const std::string& key, const T value) {
  auto it = values_.find(key);
  if (it == values_.end()) {
    values_.insert({key, std::make_unique<Datum<T>>(value)});
  } else {
    throw std::out_of_range("Datum \"" + key + "\" alredy exists.");
  }
}

template void jino::Data::setValue<std::int8_t>(const std::string&, const std::int8_t);
template void jino::Data::setValue<std::int16_t>(const std::string&, const std::int16_t);
template void jino::Data::setValue<std::int32_t>(const std::string&, const std::int32_t);
template void jino::Data::setValue<std::int64_t>(const std::string&, const std::int64_t);
template void jino::Data::setValue<std::uint8_t>(const std::string&, const std::uint8_t);
template void jino::Data::setValue<std::uint16_t>(const std::string&, const std::uint16_t);
template void jino::Data::setValue<std::uint32_t>(const std::string&, const std::uint32_t);
template void jino::Data::setValue<std::uint64_t>(const std::string&, const std::uint64_t);
template void jino::Data::setValue<float>(const std::string&, const float);
template void jino::Data::setValue<double>(const std::string&, const double);
template void jino::Data::setValue<std::string>(const std::string&, const std::string);

template <typename T>
T jino::Data::getValue(const std::string& key) const {
  auto it = values_.find(key);
  if (it == values_.end()) {
    throw std::out_of_range("Datum \"" + key + "\" not found.");
  }
  const DatumBase* baseDatum = it->second.get();
  if (const auto* datum = dynamic_cast<const Datum<T>*>(baseDatum)) {
    return datum->getValue();
  }
  try {
    if (baseDatum) {
      std::any raw = baseDatum->getRawValue();
      if (raw.has_value()) {
        if constexpr (std::is_arithmetic_v<T>) {
          if (raw.type() == typeid(std::int8_t))
            return static_cast<T>(std::any_cast<std::int8_t>(raw));
          if (raw.type() == typeid(std::uint8_t))
            return static_cast<T>(std::any_cast<std::uint8_t>(raw));
          if (raw.type() == typeid(std::int16_t))
            return static_cast<T>(std::any_cast<std::int16_t>(raw));
          if (raw.type() == typeid(std::uint16_t))
            return static_cast<T>(std::any_cast<std::uint16_t>(raw));
          if (raw.type() == typeid(std::int32_t))
            return static_cast<T>(std::any_cast<std::int32_t>(raw));
          if (raw.type() == typeid(std::uint32_t))
            return static_cast<T>(std::any_cast<std::uint32_t>(raw));
          if (raw.type() == typeid(std::int64_t))
            return static_cast<T>(std::any_cast<std::int64_t>(raw));
          if (raw.type() == typeid(std::uint64_t))
            return static_cast<T>(std::any_cast<std::uint64_t>(raw));
          if (raw.type() == typeid(float))
            return static_cast<T>(std::any_cast<float>(raw));
          if (raw.type() == typeid(double))
            return static_cast<T>(std::any_cast<double>(raw));
        } else if constexpr (std::is_same_v<T, std::string>) {
          if (raw.type() == typeid(std::string))  return std::any_cast<std::string>(raw);
        }
      }
    }
  } catch (const std::bad_any_cast&) {
    throw std::runtime_error("Invalid type conversion in getValue.");
  }

  throw std::runtime_error("Unsupported conversion in getValue.");
}

template std::int8_t jino::Data::getValue<std::int8_t>(const std::string&) const;
template std::int16_t jino::Data::getValue<std::int16_t>(const std::string&) const;
template std::int32_t jino::Data::getValue<std::int32_t>(const std::string&) const;
template std::int64_t jino::Data::getValue<std::int64_t>(const std::string&) const;
template std::uint8_t jino::Data::getValue<std::uint8_t>(const std::string&) const;
template std::uint16_t jino::Data::getValue<std::uint16_t>(const std::string&) const;
template std::uint32_t jino::Data::getValue<std::uint32_t>(const std::string&) const;
template std::uint64_t jino::Data::getValue<std::uint64_t>(const std::string&) const;
template float jino::Data::getValue<float>(const std::string&) const;
template double jino::Data::getValue<double>(const std::string&) const;
template std::string jino::Data::getValue<std::string>(const std::string&) const;

void jino::Data::forEachDatum(const std::function<void(const std::string&,
                              DatumBase* const)>& callback) const {
  for (const auto& [name, datum] : values_) {
    callback(name, datum.get());
  }
}

std::uint64_t jino::Data::size() const {
  return values_.size();
}

std::uint8_t jino::Data::contains(const std::string& key) const {
  auto it = values_.find(key);
  if (it != values_.end()) {
    return true;
  } else {
    return false;
  }
}

void jino::Data::erase(const std::string& key) {
  auto it = values_.find(key);
  if (it != values_.end()) {
    values_.erase(it);
  } else {
    throw std::out_of_range("Datum \"" + key + "\" not found.");
  }
}

void jino::Data::clear() {
  for (auto& [key, valPtr] : values_) {
    valPtr.reset();
  }
  values_.clear();
}
