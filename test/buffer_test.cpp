/***************************************************************************************************
* JSON In NetCDF Out                                                                               *
*                                                                                                  *
* (C) Copyright 2025, Phil Underwood. All rights reserved.                                         *
*                                                                                                  *
* This program is free software: you can redistribute it and/or modify it under the terms of the   *
* GNU General Public License as published by the Free Software Foundation, either version 3 of the *
* License, or (at your option) any later version.                                                  *
*                                                                                                  *
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;        *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See    *
* the GNU General Public License for more details.                                                 *
*                                                                                                  *
* You should have received a copy of the GNU General Public License along with this program. If    *
* not, see <https://www.gnu.org/licenses/>.                                                        *
***************************************************************************************************/

#include <iostream>

#include "Buffer.h"
#include "Constants.h"
#include "Data.h"
#include "FileReader.h"

long double calcIncrement(const float min, const float max, const std::uint64_t timeSteps) {
  if (min > max) {
    throw std::invalid_argument("min cannot be greater than max");
  }
  if (timeSteps == 0) {
    throw std::invalid_argument("Time steps must be greater than zero...");
  }
  return static_cast<long double>(max - min) / static_cast<long double>(timeSteps - 1);
}

std::uint64_t divide(const std::uint64_t numerator, const std::uint64_t denominator) {
  if (numerator == 0) {
    throw std::invalid_argument("Numerator must be greater than zero...");
  }
  if (denominator == 0) {
    throw std::invalid_argument("Division by zero is not allowed...");
  }
  long double result = static_cast<long double>(numerator) / static_cast<long double>(denominator);
  return static_cast<std::uint64_t>(std::round(result));  // Round to nearest integer
}

int main() {
  jino::Data params;
  jino::FileReader fileReader;
  fileReader.getParams(params);

  const std::uint64_t maxTimeSteps = params.getValue<std::uint64_t>(jino::consts::kMaxTimeSteps);
  const std::uint64_t samplingRate = params.getValue<std::uint64_t>(jino::consts::kSamplingRate);

  const long double yMin = params.getValue<float>(jino::consts::kYMin);
  const long double yMax = params.getValue<float>(jino::consts::kYMax);

  const long double yInc = calcIncrement(yMin, yMax, maxTimeSteps);
  const std::uint64_t dataSize = divide(maxTimeSteps, samplingRate);

  jino::Buffer<double> buffer(dataSize);

  long double y;
  for (std::uint64_t t = 0; t < maxTimeSteps; ++t) {
    y = yMin + t * yInc;
    if (t % samplingRate == 0) {
      buffer.setNext() = y;
    }
  }

  for (std::uint64_t i = 0; i < buffer.size(); ++i) {
    std::cout << buffer.getNext() << std::endl;
  }

  return 0;
}
