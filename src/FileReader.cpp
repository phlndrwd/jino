/******************************************************************************
* Buffered Output Thread                                                      *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "FileReader.h"

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

bot::FileReader::FileReader() {}

void bot::FileReader::read(std::string& path, std::string& text) {
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
  } catch (const std::exception& error) {
    std::cout << "ERROR: Could not access file \"" << path << "\"..." << std::endl;
    std::cerr << error.what() << std::endl;
  }
}

void bot::FileReader::getParams(bot::Parameters& params) {
  std::string text;
  std::string paramsPath = consts::kInputPath + consts::kParamFile;
  read(paramsPath, text);
  try {
    json jsonData = json::parse(text);
    for (std::uint64_t i = 0; i < consts::kParamNames.size(); ++i) {
      const std::string& paramName = consts::kParamNames[i];
      const std::uint8_t paramType = consts::kParamTypes[i];
      if (jsonData.contains(paramName)) {
        setParam(params, paramName, paramType, jsonData[paramName]);
      } else {
        throw std::out_of_range("JSON key \"" + paramName + "\" not found.");
      }
    }
  } catch (const std::exception& error) {
    std::cout << "ERROR: Input file not formatted correctly..." << std::endl;
    std::cerr << error.what() << std::endl;
  }
}

void bot::FileReader::setParam(Parameters& params, const std::string& paramName,
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
    case consts::eString: {
      setParam(params, paramName, static_cast<std::string>(jsonValue));
      break;
    }
  }
}

template <typename T>
void bot::FileReader::setParam(Parameters& params, const std::string& paramName, const T& value) {
  params.setValue(paramName, value);
}
