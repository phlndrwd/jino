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

#include <cstdint>
#include <string>

namespace bot {
namespace consts {

  enum eDataTypes : std::int8_t {
    eInt8,
    eInt16,
    eInt32,
    eInt64,
    eFloat,
    eDouble,
    eString,
    eNumberOfDataTypes
  };

  constexpr std::string kColourMapOpts[6] = {
    "blackbody",
    "coolwarm",
    "greyscale",
    "plasma",
    "turbo",
    "viridis"
  };

  const char kSeparator = ',';

  const unsigned kColourDepth = 255;
  const unsigned kColourMapSize = 256;

  const unsigned kMaxColourSize = 3;

  const std::size_t kMaxFileSizeInBytes = 1048576;  // 1MB

  // Parameter names
  constexpr std::string kMaxTimeSteps = "maxTimeSteps";
  constexpr std::string kSamplingRate = "samplingRate";
  constexpr std::string kYMin = "yMin";
  constexpr std::string kYMax = "yMax";

  // Other strings
  constexpr std::string kSpace = " ";
  constexpr std::string kBigSpace = "  ";
  constexpr std::string kInputPath = "./input/";
  constexpr std::string kParamFile = "params.json";

}  // namespace consts
}  // namespace enums

#endif // CONSTANTS_H
