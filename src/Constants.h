/******************************************************************************
* Buffered Output Thread                                                      *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef SRC_CONSTANTS_H_
#define SRC_CONSTANTS_H_

#include <array>
#include <cstdint>
#include <string>

namespace bot {
namespace consts {
enum eParams : std::uint8_t {
  eMaxTimeSteps,
  eSamplingRate,
  eYMin,
  eYMax,
  eNumberOfParams
};

enum eDataTypes : std::uint8_t {
  eInt8,
  eInt16,
  eInt32,
  eInt64,
  eUInt8,
  eUInt16,
  eUInt32,
  eUInt64,
  eFloat,
  eDouble,
  eLongDouble,
  eString,
  eNumberOfDataTypes
};

const std::size_t kMaxFileSizeInBytes = 1048576;  // 1MB

// Other strings
constexpr std::string kSeparator = ", ";
constexpr std::string kInputPath = "./input/";
constexpr std::string kParamFile = "params.json";

// Parameter names
constexpr std::string kMaxTimeSteps = "MaxTimeSteps";
constexpr std::string kSamplingRate = "SamplingRate";
constexpr std::string kYMin = "YMin";
constexpr std::string kYMax = "YMax";

const std::array<std::string, eNumberOfParams> kParamNames = {
  kMaxTimeSteps,
  kSamplingRate,
  kYMin,
  kYMax
};

const std::array<std::uint8_t, eNumberOfParams> kParamTypes = {
  eUInt64,
  eUInt64,
  eFloat,
  eFloat
};
}  // namespace consts
}  // namespace bot

#endif  // SRC_CONSTANTS_H_
