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

#include "JsonReader.h"

#include <fstream>
#include <iostream>
#include <string>

#include "Constants.h"

namespace {
std::streamsize getFileSize(const std::string& path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file) {
    throw std::ios_base::failure("Could not open file.");
  }
  return file.tellg();
}

std::uint8_t strCompare(const std::string& str1, const std::string& str2) {
  return str1.size() == str2.size() && std::equal(str1.begin(), str1.end(), str2.begin(),
                                                  [](unsigned char c1, unsigned char c2) {
    return std::tolower(c1) == std::tolower(c2);
  });
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
    if (static_cast<std::size_t>(sz) <= consts::kMaxFileSizeInBytes) {
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

void jino::JsonReader::readParams(jino::Data& params) {
  std::string text;
  std::string path = consts::kInputPath + consts::kParamsFile;
  readText(path, text);
  try {
    json jsonData = json::parse(text);  // Arranged alphabetically
    if (jsonData.is_object() && jsonData.size() == consts::kParamNames.size()) {
      for (std::uint64_t i = 0; i < jsonData.size(); ++i) {
        const std::string& paramName = consts::kParamNames.at(i);
        const std::uint8_t paramType = consts::kParamTypes.at(i);
        if (jsonData.contains(paramName)) {
          setValue(params, paramName, paramType, jsonData[paramName]);
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

void jino::JsonReader::readAttrs(jino::Data& attrs) {
  std::string text;
  std::string path = consts::kInputPath + consts::kAttrsFile;
  readText(path, text);
  try {
    json jsonData = json::parse(text);  // Arranged alphabetically
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
            setValue(attrs, key, value.get<float>());
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

void jino::JsonReader::setValue(Data& params, const std::string& paramName,
                                const std::uint8_t paramType, const nlohmann::json& jsonValue) {
  switch (paramType) {
    case consts::eInt8: {
      setValue(params, paramName, static_cast<std::int8_t>(jsonValue));
      break;
    }
    case consts::eInt16: {
      setValue(params, paramName, static_cast<std::int16_t>(jsonValue));
      break;
    }
    case consts::eInt32: {
      setValue(params, paramName, static_cast<std::int32_t>(jsonValue));
      break;
    }
    case consts::eInt64: {
      setValue(params, paramName, static_cast<std::int64_t>(jsonValue));
      break;
    }
    case consts::eUInt8: {
      setValue(params, paramName, static_cast<std::uint8_t>(jsonValue));
      break;
    }
    case consts::eUInt16: {
      setValue(params, paramName, static_cast<std::uint16_t>(jsonValue));
      break;
    }
    case consts::eUInt32: {
      setValue(params, paramName, static_cast<std::uint32_t>(jsonValue));
      break;
    }
    case consts::eUInt64: {
      setValue(params, paramName, static_cast<std::uint64_t>(jsonValue));
      break;
    }
    case consts::eFloat: {
      setValue(params, paramName, static_cast<float>(jsonValue));
      break;
    }
    case consts::eDouble: {
      setValue(params, paramName, static_cast<double>(jsonValue));
      break;
    }
    case consts::eString: {
      setValue(params, paramName, static_cast<std::string>(jsonValue));
      break;
    }
  }
}

template <typename T>
void jino::JsonReader::setValue(Data& params, const std::string& paramName, const T& value) {
  params.setValue(paramName, value);
}
