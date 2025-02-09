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
#include <cstdint>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "nlohmann/json.hpp"

#include "Buffer.h"
#include "Buffers.h"
#include "JsonReader.h"
#include "OutputThread.h"

using json = nlohmann::json;

class Piston {
 public:
  double temperature_;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Piston, temperature_)
};

class Engine {
 public:
  std::vector<Piston> pistons_;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Engine, pistons_)
};

class Car {
 public:
  std::string make_;
  std::string model_;
  Engine engine_;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Car, make_, model_, engine_)
};

class Garage {
 public:
  std::vector<Car> cars_;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Garage, cars_)
};

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

std::int32_t main() {
  std::cout << "Deserialising data from file..." << std::endl;
  jino::JsonReader reader;
  Garage garage = reader.readState<Garage>();

  std::cout << "Creating pseudo-model data..." << std::endl;
  jino::Data attrs;
  jino::Data params;
  reader.readAttrs(attrs);
  reader.readParams(params);

  jino::OutputThread output;
  jino::NetCDFData data;

  data.addDateToData(&attrs, output.getDate());
  data.addData(&params);

  const std::uint64_t maxTimeStep = params.getValue<std::uint64_t>(jino::consts::kMaxTimeStep);
  const std::uint64_t samplingRate = params.getValue<std::uint64_t>(jino::consts::kSamplingRate);

  const long double yMin = params.getValue<float>(jino::consts::kYMin);
  const long double yMax = params.getValue<float>(jino::consts::kYMax);

  const long double yInc = calcIncrement(yMin, yMax, maxTimeStep);
  const std::uint64_t dataSize = calcDataSize(maxTimeStep, samplingRate);

  data.addDimension("dataSize", dataSize);

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

  std::cout << "Running pseudo-model loop..." << std::endl;
  output.initNetCDF();
  output.writeMetadata(data);
  for (t = 0; t <= maxTimeStep; ++t) {
    y = yMin + t * yInc;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    if (t % samplingRate == 0) {
      //std::cout << "t=" << t << std::endl;
      jino::Buffers::get().record();
      output.writeDatums(data);
      r = r * 2;
    }
  }
  std::uint8_t writeState = params.getValue<std::uint8_t>(jino::consts::kWriteState);
  if (writeState == true) {
    std::cout << "Serialise objects to JSON and write to file..." << std::endl;
    output.writeState(garage);
  }
  std::cout << "Closing NetCDF..." << std::endl;
  output.closeNetCDF();
  std::cout << "Waiting for completion..." << std::endl;
  output.stop();
  std::cout << "Complete." << std::endl;

  return 0;
}
