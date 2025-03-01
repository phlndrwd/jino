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

#ifndef INCLUDE_DATA_H_
#define INCLUDE_DATA_H_

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>

#include "Datum.h"
#include "DatumBase.h"

namespace jino {
class Data {
 public:
  Data() = default;

  jino::DatumBase& operator[](const std::string&);
  const jino::DatumBase& operator[](const std::string&) const;

  template <typename T>
  void setValue(const std::string&, const T);

    template <typename T>
    T getValue(const std::string& key) const {
        auto it = values_.find(key);
        if (it != values_.end()) {
            // Try direct type match
            if (Datum<T>* datum = dynamic_cast<Datum<T>*>(it->second.get())) {
                return datum->getValue();
            }

            // Try implicit numeric conversion
            return tryConvert<T>(it->second.get());
        } else {
            throw std::out_of_range("Datum \"" + key + "\" not found.");
        }
    }

  void forEachDatum(const std::function<void(const std::string&, DatumBase* const)>&) const;

  std::uint64_t size() const;

  std::uint8_t contains(const std::string&) const;

  void erase(const std::string&);
  void clear();

 private:
  template <typename T, typename StoredT>
  static T safeConvert(StoredT value) {
    if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<StoredT>) {
      return static_cast<T>(value);
    } else {
      throw std::runtime_error("Invalid type conversion.");
    }
  }

  template <typename T>
  T tryConvert(DatumBase* baseDatum) const {
    #define TRY_CONVERT(TYPE) \
    if (Datum<TYPE>* datum = dynamic_cast<Datum<TYPE>*>(baseDatum)) { \
        return safeConvert<T>(datum->getValue()); \
    }

    TRY_CONVERT(std::uint8_t)
    TRY_CONVERT(std::int8_t)
    TRY_CONVERT(std::uint16_t)
    TRY_CONVERT(std::int16_t)
    TRY_CONVERT(std::uint32_t)
    TRY_CONVERT(std::int32_t)
    TRY_CONVERT(std::uint64_t)
    TRY_CONVERT(std::int64_t)
    TRY_CONVERT(float)
    TRY_CONVERT(double)
    TRY_CONVERT(std::string)

    throw std::runtime_error("Type mismatch or invalid cast.");
  }

  std::map<const std::string, std::unique_ptr<DatumBase>> values_;
};
}  // namespace jino

#endif  // INCLUDE_DATA_H_
