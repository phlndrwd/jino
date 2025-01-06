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
std::streamsize getFileSize(const std::string& filePath) {
  std::ifstream file(filePath, std::ios::binary | std::ios::ate);
  if (!file) {
    throw std::ios_base::failure("Could not open file.");
  }
  return file.tellg();
}
}  // Anonymous namespace

std::uint8_t jino::JsonReader::readText(std::string& path, std::string& text) {
  try {
    std::ifstream fileIn(path);
    fileIn.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    if (!fileIn.is_open()) {
      throw std::ios_base::failure("Failed to open file.");
    }
    std::streamsize sz = getFileSize(path);
    if (static_cast<std::size_t>(sz) <= consts::kMaxFileSizeInBytes) {
      text.assign((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
    } else {
      std::cout << "ERROR: Input file \"" << path << "\" is too large..." << std::endl;
    }
    fileIn.close();
    return true;
  } catch (const std::exception& error) {
    std::cout << "ERROR: Could not access file \"" << path << "\"..." << std::endl;
    std::cerr << error.what() << std::endl;
    return false;
  }
}

std::uint8_t jino::JsonReader::readParams(jino::Data& params) {
  std::string text;
  std::string path = consts::kInputPath + consts::kParamsFile;
  if (readText(path, text) == true) {
    try {
      json jsonData = json::parse(text);  // Arranged alphabetically
      if (jsonData.is_object() && jsonData.size() == consts::kParamNames.size()) {
        for (std::uint64_t i = 0; i < jsonData.size(); ++i) {
          const std::string& paramName = consts::kParamNames.at(i);
          const std::uint8_t paramType = consts::kParamTypes.at(i);

          if (jsonData.contains(paramName)) {
            setParam(params, paramName, paramType, jsonData[paramName]);
          } else {
            throw std::out_of_range("Parameter \"" + paramName + "\" not found.");
          }
        }
      } else {
        throw std::runtime_error("Unexpected file format.");
      }
    } catch (const std::exception& error) {
      std::cout << "ERROR: Params file not formatted correctly..." << std::endl;
      std::cerr << error.what() << std::endl;
      return false;
    }
  }
  return true;
}


void jino::JsonReader::readAttrs(jino::Data& attrs) {
  std::string text;
  std::string path = consts::kInputPath + consts::kAttrsFile;

  if (readText(path, text) == true) {
    try {
    json jsonData = json::parse(text);

      if (jsonData.is_object()) {
        for (std::uint64_t i = 0; i < jsonData.size(); ++i) {
          auto item = std::next(jsonData.begin(), i);
          std::string key = static_cast<std::string>(item.key());
          std::cout << "KEY: " << key << std::endl;
        }
      }


    if (jsonData.is_object()) {
      for (auto it = jsonData.begin(); it != jsonData.end(); ++it) {
        std::cout << "Key: " << it.key() << ", Value: " << it.value() << ", Type: " << it.value().type_name() << std::endl;

        it.value().type();

        if (it.value().is_array()) {
          for (const auto& elem : it.value()) {
            std::cout << "  Array element: " << elem << std::endl;
          }
        } else if (it.value().is_object()) {
          for (auto inner_it = it.value().begin(); inner_it != it.value().end(); ++inner_it) {
            std::cout << "  Inner key: " << inner_it.key() << ", Value: " << inner_it.value() << std::endl;
          }
        }
        // Add additional processing for `attrs` here
      }
    } else {
      std::cerr << "Unsupported JSON structure." << std::endl;
    }
    } catch (const std::exception& error) {
      std::cout << "ERROR: Input file not formatted correctly..." << std::endl;
      std::cerr << error.what() << std::endl;
    }
  }
}

void jino::JsonReader::setParam(Data& params, const std::string& paramName,
                                const std::uint8_t paramType, const nlohmann::json& jsonValue) {
  switch (paramType) {
    case consts::eInt8: {
      setParam(params, paramName, static_cast<std::int8_t>(jsonValue));
      break;
    }
    case consts::eInt16: {
      setParam(params, paramName, static_cast<std::int16_t>(jsonValue));
      break;
    }
    case consts::eInt32: {
      setParam(params, paramName, static_cast<std::int32_t>(jsonValue));
      break;
    }
    case consts::eInt64: {
      setParam(params, paramName, static_cast<std::int64_t>(jsonValue));
      break;
    }
    case consts::eUInt8: {
      setParam(params, paramName, static_cast<std::uint8_t>(jsonValue));
      break;
    }
    case consts::eUInt16: {
      setParam(params, paramName, static_cast<std::uint16_t>(jsonValue));
      break;
    }
    case consts::eUInt32: {
      setParam(params, paramName, static_cast<std::uint32_t>(jsonValue));
      break;
    }
    case consts::eUInt64: {
      setParam(params, paramName, static_cast<std::uint64_t>(jsonValue));
      break;
    }
    case consts::eFloat: {
      setParam(params, paramName, static_cast<float>(jsonValue));
      break;
    }
    case consts::eDouble: {
      setParam(params, paramName, static_cast<double>(jsonValue));
      break;
    }
    case consts::eLongDouble: {
      setParam(params, paramName, static_cast<long double>(jsonValue));
      break;
    }
    case consts::eString: {
      setParam(params, paramName, static_cast<std::string>(jsonValue));
      break;
    }
  }
}

template <typename T>
void jino::JsonReader::setParam(Data& params, const std::string& paramName, const T& value) {
  params.setValue(paramName, value);
}
