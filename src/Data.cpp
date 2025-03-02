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
  if (it != values_.end()) {
    if (Datum<T>* datum = dynamic_cast<Datum<T>*>(it->second.get())) {
      return datum->getValue();
    }
    return tryConvert<T>(it->second.get());
  } else {
    throw std::out_of_range("Datum \"" + key + "\" not found.");
  }
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

template <typename T, typename StoredT>
T jino::Data::safeConvert(StoredT value) {
  if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<StoredT>) {
    return static_cast<T>(value);
} else {
    throw std::runtime_error("Invalid type conversion.");
  }
}

template std::int8_t jino::Data::safeConvert<std::int8_t, std::uint8_t>(std::uint8_t);
template std::int16_t jino::Data::safeConvert<std::int16_t, std::uint16_t>(std::uint16_t);
template std::int32_t jino::Data::safeConvert<std::int32_t, std::uint32_t>(std::uint32_t);
template std::int64_t jino::Data::safeConvert<std::int64_t, std::uint64_t>(std::uint64_t);
template float jino::Data::safeConvert<float, double>(double);
template double jino::Data::safeConvert<double, float>(float);

template <>
std::string jino::Data::tryConvertHelper<std::string, std::string>(DatumBase* baseDatum) const {
  Datum<std::string>* datum = dynamic_cast<Datum<std::string>*>(baseDatum);
  if (datum) {
    return datum->getValue();
  }
  return "";
}

template <typename T>
T jino::Data::tryConvert(DatumBase* baseDatum) const {
  if (T result = tryConvertHelper<T, std::uint8_t>(baseDatum); result != T{}) {
    return result;
  }
  if (T result = tryConvertHelper<T, std::uint16_t>(baseDatum); result != T{}) {
    return result;
  }
  if (T result = tryConvertHelper<T, std::uint32_t>(baseDatum); result != T{}) {
    return result;
  }
  if (T result = tryConvertHelper<T, std::uint64_t>(baseDatum); result != T{}) {
    return result;
  }
  if (T result = tryConvertHelper<T, std::int8_t>(baseDatum); result != T{})  {
    return result;
  }
  if (T result = tryConvertHelper<T, std::int16_t>(baseDatum); result != T{}) {
    return result;
  }
  if (T result = tryConvertHelper<T, std::int32_t>(baseDatum); result != T{}) {
    return result;
  }
  if (T result = tryConvertHelper<T, std::int64_t>(baseDatum); result != T{}) {
    return result;
  }
  if (T result = tryConvertHelper<T, float>(baseDatum); result != T{}) {
    return result;
  }
  if (T result = tryConvertHelper<T, double>(baseDatum); result != T{}) {
    return result;
  }
  if (T result = tryConvertHelper<T, std::string>(baseDatum); result != T{}) {
    return result;
  }
  throw std::runtime_error("Failed to convert datum");
}

template std::int8_t jino::Data::tryConvert<std::int8_t>(DatumBase*) const;
template std::int16_t jino::Data::tryConvert<std::int16_t>(DatumBase*) const;
template std::int32_t jino::Data::tryConvert<std::int32_t>(DatumBase*) const;
template std::int64_t jino::Data::tryConvert<std::int64_t>(DatumBase*) const;
template std::uint8_t jino::Data::tryConvert<std::uint8_t>(DatumBase*) const;
template std::uint16_t jino::Data::tryConvert<std::uint16_t>(DatumBase*) const;
template std::uint32_t jino::Data::tryConvert<std::uint32_t>(DatumBase*) const;
template std::uint64_t jino::Data::tryConvert<std::uint64_t>(DatumBase*) const;
template float jino::Data::tryConvert<float>(DatumBase*) const;
template double jino::Data::tryConvert<double>(DatumBase*) const;
template std::string jino::Data::tryConvert<std::string>(DatumBase*) const;

template <typename T, typename StoredT>
T jino::Data::tryConvertHelper(DatumBase* baseDatum) const {
  if (Datum<StoredT>* datum = dynamic_cast<Datum<StoredT>*>(baseDatum)) {
    return safeConvert<T>(datum->getValue());
  }
  return T{};
}

template std::int8_t jino::Data::tryConvertHelper<std::int8_t, std::uint8_t>(DatumBase*) const;
template std::int8_t jino::Data::tryConvertHelper<std::int8_t, std::int8_t>(DatumBase*) const;
template std::int16_t jino::Data::tryConvertHelper<std::int16_t, std::uint16_t>(DatumBase*) const;
template std::int16_t jino::Data::tryConvertHelper<std::int16_t, std::int16_t>(DatumBase*) const;
template std::int32_t jino::Data::tryConvertHelper<std::int32_t, std::uint32_t>(DatumBase*) const;
template std::int32_t jino::Data::tryConvertHelper<std::int32_t, std::int32_t>(DatumBase*) const;
template std::int64_t jino::Data::tryConvertHelper<std::int64_t, std::uint64_t>(DatumBase*) const;
template std::int64_t jino::Data::tryConvertHelper<std::int64_t, std::int64_t>(DatumBase*) const;
template float jino::Data::tryConvertHelper<float, float>(DatumBase*) const;
template double jino::Data::tryConvertHelper<double, double>(DatumBase*) const;
