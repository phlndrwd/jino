/******************************************************************************
* Buffered Output Thread (BOT)                                                *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <array>
#include <cstdint>
#include <string>

namespace bot {
namespace consts {

  enum eParams : std::int8_t {
    eMaxTimeSteps,
    eSamplingRate,
    eYMin,
    eYMax,
    eNumberOfParams
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

}  // namespace consts
}  // namespace enums

#endif // CONSTANTS_H
