/******************************************************************************
* Buffered Output Thread                                                      *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include <iostream>

#include "Constants.h"
#include "FileReader.h"
#include "Parameters.h"

int main() {
  std::cout << "Reading parameters..." << std::endl;
  bot::Parameters params;
  bot::FileReader fileReader;
  fileReader.getParams(params);

  for (std::uint64_t i = 0; i < params.size(); i++) {
    std::cout << params[i].getValueStr() << std::endl;
  }

  std::uint64_t timeSteps = params.getValue<std::uint64_t>(bot::consts::kMaxTimeSteps);
  std::uint64_t samplingRate = params.getValue<std::uint64_t>(bot::consts::kSamplingRate);
  float yMin = params.getValue<float>(bot::consts::kYMin);
  float yMax = params.getValue<float>(bot::consts::kYMax);

  std::cout << bot::consts::kMaxTimeSteps << bot::consts::kSeparator << timeSteps << std::endl;
  std::cout << bot::consts::kSamplingRate << bot::consts::kSeparator << samplingRate << std::endl;
  std::cout << bot::consts::kYMin << bot::consts::kSeparator << yMin << std::endl;
  std::cout << bot::consts::kYMax << bot::consts::kSeparator << yMax << std::endl;

  std::cout << "Done." << std::endl;
  return 0;
}
