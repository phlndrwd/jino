/**********************************************************************************************
* Jino (JSON In NetCDF Out).                                                                  *
*                                                                                             *
* (C) Copyright 2025, Phil Underwood.                                                         *
*                                                                                             *
* Jino is free software: you can redistribute it and/or modify it under the terms of the GNU  *
* Lesser General Public License as published by the Free Software Foundation, either version  *
* 3 of the License, or (at your option) any later version.                                    *
*                                                                                             *
* Jino is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without   *
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the   *
* GNU Lesser General Public License for more details.                                         *
*                                                                                             *
* You should have received a copy of the GNU Lesser General Public License along with Jino.   *
* If not, see <https://www.gnu.org/licenses/>.                                                *
**********************************************************************************************/

#include <chrono>
#include <iostream>
#include <thread>

#include "Buffer.h"
#include "Buffers.h"
#include "Constants.h"
#include "Data.h"
#include "JsonReader.h"
#include "NetCDFData.h"
#include "Output.h"

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
                       static_cast<long double>(samplingRate);
  return static_cast<std::uint64_t>(std::ceil(result) + 1);
}

int main() {
  jino::Data attrs;
  jino::Data params;
  jino::JsonReader reader;

  reader.readAttrs(attrs);
  reader.readParams(params);

  jino::Output output;
  jino::NetCDFData data;

  data.addDateToData(&attrs, output.getDate());
  data.addData(&params);

  const std::uint64_t maxTimeStep = params.getValue<std::uint64_t>(jino::consts::kMaxTimeStep);
  const std::uint64_t samplingRate = params.getValue<std::uint64_t>(jino::consts::kSamplingRate);

  const long double yMin = params.getValue<float>(jino::consts::kYMin);
  const long double yMax = params.getValue<float>(jino::consts::kYMax);

  const long double yInc = calcIncrement(yMin, yMax, maxTimeStep);
  const std::uint64_t dataSize = calcDataSize(maxTimeStep, samplingRate);

  data.addDimension("dataSize", dataSize, true);

  double y = 0;
  std::uint64_t t = 0;
  std::uint64_t r = 1;

  auto yBuffer1 = jino::Buffer<double>("y", "group01", dataSize, y);
  auto yBuffer2 = jino::Buffer<double>("y", "group02", dataSize, y);
  auto yBuffer3 = jino::Buffer<double>("y", "group03", dataSize, y);
  auto yBuffer4 = jino::Buffer<double>("y", "group04", dataSize, y);
  auto yBuffer5 = jino::Buffer<double>("y", "group05", dataSize, y);
  auto yBuffer6 = jino::Buffer<double>("y", "group06", dataSize, y);
  auto yBuffer7 = jino::Buffer<double>("y", "group07", dataSize, y);
  auto yBuffer8 = jino::Buffer<double>("y", "group08", dataSize, y);
  auto yBuffer9 = jino::Buffer<double>("y", "group09", dataSize, y);
  auto yBuffer10 = jino::Buffer<double>("y", "group10", dataSize, y);

  auto tBuffer1 = jino::Buffer<std::uint64_t>("t", "group01", dataSize, t);
  auto tBuffer2 = jino::Buffer<std::uint64_t>("t", "group02", dataSize, t);
  auto tBuffer3 = jino::Buffer<std::uint64_t>("t", "group03", dataSize, t);
  auto tBuffer4 = jino::Buffer<std::uint64_t>("t", "group04", dataSize, t);
  auto tBuffer5 = jino::Buffer<std::uint64_t>("t", "group05", dataSize, t);
  auto tBuffer6 = jino::Buffer<std::uint64_t>("t", "group06", dataSize, t);
  auto tBuffer7 = jino::Buffer<std::uint64_t>("t", "group07", dataSize, t);
  auto tBuffer8 = jino::Buffer<std::uint64_t>("t", "group08", dataSize, t);
  auto tBuffer9 = jino::Buffer<std::uint64_t>("t", "group09", dataSize, t);
  auto tBuffer10 = jino::Buffer<std::uint64_t>("t", "group10", dataSize, t);

  auto rBuffer1 = jino::Buffer<std::uint64_t>("r01", dataSize, r);
  auto rBuffer2 = jino::Buffer<std::uint64_t>("r02", dataSize, r);
  auto rBuffer3 = jino::Buffer<std::uint64_t>("r03", dataSize, r);
  auto rBuffer4 = jino::Buffer<std::uint64_t>("r04", dataSize, r);
  auto rBuffer5 = jino::Buffer<std::uint64_t>("r05", dataSize, r);
  auto rBuffer6 = jino::Buffer<std::uint64_t>("r06", dataSize, r);
  auto rBuffer7 = jino::Buffer<std::uint64_t>("r07", dataSize, r);
  auto rBuffer8 = jino::Buffer<std::uint64_t>("r08", dataSize, r);
  auto rBuffer9 = jino::Buffer<std::uint64_t>("r09", dataSize, r);
  auto rBuffer10 = jino::Buffer<std::uint64_t>("r10", dataSize, r);

  output.initNetCDF(jino::consts::eSingleThread);
  output.getNetCDF().writeMetadata(data);
  for (t = 0; t <= maxTimeStep; ++t) {
    y = yMin + t * yInc;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    if (t % samplingRate == 0) {
      std::cout << "t=" << t << std::endl;
      jino::Buffers::get().record();
      output.getNetCDF().writeDatums(data);
      r = r * 2;
    }
  }
  output.getNetCDF().closeFile();

  return 0;
}
