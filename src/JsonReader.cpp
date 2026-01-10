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

#include "JsonReader.h"

#include <fstream>
#include <iostream>
#include <limits>
#include <stdfloat>
#include <string>
#include <vector>

#include "Constants.h"

namespace {
std::streamsize getFileSize(const std::string& path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file) {
    throw std::ios_base::failure("Could not open file.");
  }
  return file.tellg();
}
}  // Anonymous namespace

void jino::JsonReader::readText(const std::string& path, std::string& text) {
  try {
    std::ifstream fileIn(path);
    fileIn.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    if (!fileIn.is_open()) {
      throw std::ios_base::failure("Could not open file.");
    }
    std::streamsize sz = getFileSize(path);
    if (static_cast<std::uint64_t>(sz) <= consts::kMaxFileSizeInBytes) {
      text.assign((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
    } else {
      throw std::runtime_error("File is too large.");
    }
    fileIn.close();
  } catch (const std::exception& error) {
    std::cout << "ERROR: Could not access file \"" << path << "\"..." << std::endl;
    std::cerr << error.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void jino::JsonReader::readParams(jino::Data& params, const std::string& path,
                                  const std::vector<std::string>& paramNames) {
  std::string text;
  readText(path, text);
  try {
    nlohmann::json jsonData = nlohmann::json::parse(text);  // Arranged alphabetically
    if (jsonData.is_object() && jsonData.size() == paramNames.size()) {
      for (std::uint64_t i = 0; i < jsonData.size(); ++i) {
        const std::string& paramName = paramNames.at(i);
        if (jsonData.contains(paramName)) {
          setValue(params, paramName, jsonData[paramName]);
        } else {
          throw std::out_of_range("Required parameter \"" + paramName + "\" not found in file.");
        }
      }
    } else {
      throw std::runtime_error("Incorrect file format.");
    }
  } catch (const std::exception& error) {
    std::cout << "ERROR: Params file not formatted correctly..." << std::endl;
    std::cerr << error.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void jino::JsonReader::readAttrs(jino::Data& attrs, const std::string& path) {
  std::string text;
  readText(path, text);
  try {
    nlohmann::json jsonData = nlohmann::json::parse(text);  // Arranged alphabetically
    if (jsonData.is_object()) {
      for (auto it = jsonData.begin(); it != jsonData.end(); ++it) {
        const std::string& key = it.key();
        const auto& value = it.value();
        if (value.is_string()) {
          setValue(attrs, key, value.get<std::string>());
        } else if (value.is_number()) {
          if (value.is_number_integer()) {
            setValue(attrs, key, value.get<std::int32_t>());
          } else if (value.is_number_float()) {
            setValue(attrs, key, value.get<std::float32_t>());
          }
        } else if (value.is_boolean()) {
          setValue(attrs, key, value.get<std::uint8_t>());
        } else {
          throw std::runtime_error("Value type for key \"" + key + "\" is unsupported.");
        }
      }
    } else {
      throw std::runtime_error("Incorrect file format.");
    }
  } catch (const std::exception& error) {
    std::cout << "ERROR: Input file not formatted correctly..." << std::endl;
    std::cerr << error.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

JsonValueType jino::JsonReader::getVariant(const nlohmann::json& jsonValue) {
    if (jsonValue.is_boolean()) {
      return jsonValue.get<std::uint8_t>();
    }  else if (jsonValue.is_number_unsigned()) {
      auto val = jsonValue.get<std::uint64_t>();
      if (val <= std::numeric_limits<std::uint8_t>::max())
        return static_cast<std::uint8_t>(val);
      if (val <= std::numeric_limits<std::uint16_t>::max())
        return static_cast<std::uint16_t>(val);
      if (val <= std::numeric_limits<std::uint32_t>::max())
        return static_cast<std::uint32_t>(val);
      return val;
    } else if (jsonValue.is_number_float()) {
      std::float64_t val = jsonValue.get<std::float64_t>();
      if (val >= std::numeric_limits<std::float32_t>::lowest() &&
          val <= std::numeric_limits<std::float32_t>::max()) {
        return static_cast<std::float32_t>(val);
      } else {
        return val;
      }
    } else if (jsonValue.is_number_integer()) {
      auto val = jsonValue.get<std::int64_t>();
      if (val >= std::numeric_limits<std::int8_t>::min() &&
          val <= std::numeric_limits<std::int8_t>::max()) {
        return static_cast<std::int8_t>(val);
      } else if (val >= std::numeric_limits<std::int16_t>::min() &&
                 val <= std::numeric_limits<std::int16_t>::max()) {
        return static_cast<std::int16_t>(val);
      } else if (val >= std::numeric_limits<std::int32_t>::min() &&
                 val <= std::numeric_limits<std::int32_t>::max()) {
        return static_cast<std::int32_t>(val);
      } else {
        return val;
      }
    } else if (jsonValue.is_string()) {
      return jsonValue.get<std::string>();
    } else {
      throw std::runtime_error("Unsupported JSON value type");
    }
}

void jino::JsonReader::setValue(Data& params, const std::string& paramName,
                                const nlohmann::json& jsonValue) {
    JsonValueType value = getVariant(jsonValue);

    std::visit([&](auto&& arg) {
        setValue(params, paramName, arg);
    }, value);
}

template <typename T>
void jino::JsonReader::setValue(Data& params, const std::string& paramName, const T& value) {
  params.setValue(paramName, value);
}
