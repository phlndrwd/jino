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

#include "Buffer.h"
#include "Buffers.h"
#include "Constants.h"
#include "Data.h"
#include "JsonReader.h"

void outOfScopeTest(const std::uint64_t dataSize) {
  std::uint64_t x = 0;
  auto testBuffer = jino::Buffer<std::uint64_t>("testBuffer", dataSize, x);
}

long double calcIncrement(const float min, const float max, const std::uint64_t timeSteps) {
  if (min > max) {
    throw std::invalid_argument("min cannot be greater than max");
  }
  if (timeSteps == 0) {
    throw std::invalid_argument("Time steps must be greater than zero...");
  }
  return static_cast<long double>(max - min) / static_cast<long double>(timeSteps - 1);
}

std::uint64_t calcDataSize(const std::uint64_t maxTimeSteps, const std::uint64_t samplingRate) {
  if (maxTimeSteps == 0) {
    throw std::invalid_argument("Numerator must be greater than zero...");
  }
  if (samplingRate == 0) {
    throw std::invalid_argument("Division by zero is not allowed...");
  }
  long double result = static_cast<long double>(maxTimeSteps) /
                       static_cast<long double>(samplingRate - 1);
  return static_cast<std::uint64_t>(std::round(result));
}

int main() {
  jino::Data params;
  jino::Data attrs;
  jino::JsonReader jsonReader;
  jsonReader.readParams(params);
  jsonReader.readAttrs(attrs);

  const std::uint64_t maxTimeStep = params.getValue<std::uint64_t>(jino::consts::kMaxTimeStep);
  const std::uint64_t samplingRate = params.getValue<std::uint64_t>(jino::consts::kSamplingRate);

  const long double yMin = params.getValue<float>(jino::consts::kYMin);
  const long double yMax = params.getValue<float>(jino::consts::kYMax);

  const long double yInc = calcIncrement(yMin, yMax, maxTimeStep);
  const std::uint64_t dataSize = calcDataSize(maxTimeStep, samplingRate);

  outOfScopeTest(dataSize);

  jino::Buffers::get().addDimension("dataSize", dataSize);

  double y = 0;
  std::uint64_t t = 0;
  auto yBuffer = jino::Buffer<double>("Y", dataSize, y);
  auto tBuffer = jino::Buffer<std::uint64_t>("t", dataSize, t);
  for (t = 0; t <= maxTimeStep; ++t) {
    y = yMin + t * yInc;
    if (t % samplingRate == 0) {
      jino::Buffers::get().record();
    }
  }

  jino::Buffers::get().toFile(attrs, params);

  return 0;
}
