/******************************************************************************
* JSON In NetCDF Out                                                          *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include <iostream>

#include "Constants.h"
#include "Data.h"
#include "FileReader.h"

int main() {
  std::cout << "Reading parameters..." << std::endl;
  jino::Data params;
  jino::FileReader fileReader;
  fileReader.getParams(params);

  for (std::uint64_t i = 0; i < params.size(); i++) {
    std::cout << params[i].getValueStr() << std::endl;
  }

  std::uint64_t timeSteps = params.getValue<std::uint64_t>(jino::consts::kMaxTimeSteps);
  std::uint64_t samplingRate = params.getValue<std::uint64_t>(jino::consts::kSamplingRate);
  float yMin = params.getValue<float>(jino::consts::kYMin);
  float yMax = params.getValue<float>(jino::consts::kYMax);

  std::cout << jino::consts::kMaxTimeSteps << jino::consts::kSeparator << timeSteps << std::endl;
  std::cout << jino::consts::kSamplingRate << jino::consts::kSeparator << samplingRate << std::endl;
  std::cout << jino::consts::kYMin << jino::consts::kSeparator << yMin << std::endl;
  std::cout << jino::consts::kYMax << jino::consts::kSeparator << yMax << std::endl;

  std::cout << "Done." << std::endl;
  return 0;
}
