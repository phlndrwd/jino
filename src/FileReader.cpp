/******************************************************************************
* Buffered Output Thread (BOT)                                                *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "FileReader.h"

#include <fstream>
#include <iostream>

#include "nlohmann/json.hpp"

#include "Constants.h"

using json = nlohmann::json;

namespace {

std::streamsize getFileSize(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::ios_base::failure("Could not open file");
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
    if (jsonData.contains(consts::kMaxTimeSteps)) {
      std::uint64_t maxTimeSteps = jsonData[consts::kMaxTimeSteps];
      params.setValue(consts::kMaxTimeSteps, maxTimeSteps);
    } else {
      throw std::runtime_error("JSON key not found.");
    }
    if (jsonData.contains(consts::kSamplingRate)) {
      std::uint64_t samplingRate = jsonData[consts::kSamplingRate];
      params.setValue(consts::kSamplingRate, samplingRate);
    } else {
      throw std::runtime_error("JSON key not found.");
    }
    if (jsonData.contains(consts::kYMin)) {
      float yMin = jsonData[consts::kYMin];
      params.setValue(consts::kYMin, yMin);
    } else {
      throw std::runtime_error("JSON key not found.");
    }
    if (jsonData.contains(consts::kYMax)) {
      float yMax = jsonData[consts::kYMax];
      params.setValue(consts::kYMax, yMax);
    } else {
      throw std::runtime_error("JSON key not found.");
    }
  } catch (const std::exception& error) {
    std::cout << "ERROR: Input file not formatted correctly..." << std::endl;
    std::cerr << error.what() << std::endl;
  }
}
