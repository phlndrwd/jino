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

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "Buffer.h"
#include "Data.h"
#include "DatumBase.h"
#include "JsonReader.h"

constexpr std::string kMaxTimeStepName = "MaxTimeStep";
constexpr std::string kSamplingRateName = "SamplingRate";
constexpr std::string kWriteStateName = "WriteState";
constexpr std::string kYMinName = "YMin";
constexpr std::string kYMaxName = "YMax";

const std::vector<std::string> paramNames = {
  kMaxTimeStepName,
  kSamplingRateName,
  kWriteStateName,
  kYMinName,
  kYMaxName
};

std::uint8_t isStringInArray(const std::string str, std::vector<std::string> array) {
  if (std::find(array.begin(), array.end(), str) != array.end()) {
    return true;
  } else {
    return false;
  }
}

int main() {
  std::cout << "1. Testing file reading..." << std::endl;
  jino::Data params;
  jino::JsonReader reader;

  std::string path = jino::consts::kInputDir + jino::consts::kParamsFile;
  reader.readParams(params, path, paramNames);

  std::cout << "2. Validating read data..." << std::endl;
  assert(params.size() == paramNames.size());

  params.forEachDatum([&](const std::string& name, jino::DatumBase* const) {
    assert(params.contains(name) == true);
    assert(isStringInArray(name, paramNames));
  });

  std::cout << "3. Testing data retrieval..." << std::endl;
  static_cast<void>(params.getValue<std::uint64_t>(kMaxTimeStepName));

  const std::uint64_t samplingRate = params.getValue<std::uint64_t>(kSamplingRateName);

  static_cast<void>(params.getValue<double>(kYMinName));
  static_cast<void>(params.getValue<double>(kYMaxName));

  std::cout << "4. Testing element erasure..." << std::endl;
  params.erase(kSamplingRateName);
  assert(params.contains(kSamplingRateName) == false);
  assert(params.size() == paramNames.size() - 1);

  std::cout << "5. Testing buffer creation..." << std::endl;
  std::uint64_t i = 0;
  auto buffer = jino::Buffer<std::uint64_t>("test", samplingRate, i);
  assert(buffer.size() == samplingRate);
  for (i = 0; i < samplingRate; ++i) {
    buffer.record();
  }
  assert(buffer.at(samplingRate - 1) == samplingRate - 1);
  std::cout << "All Passed." << std::endl;

  return 0;
}
