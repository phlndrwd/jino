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

#include <array>
#include <cassert>
#include <iostream>
#include <string>

#include "Buffer.h"
#include "Constants.h"
#include "Data.h"
#include "DatumBase.h"
#include "JsonReader.h"
#include "Output.h"

std::uint8_t isStringInArray(const std::string str, std::array<std::string, 4> array) {
  if (std::find(array.begin(), array.end(), str) != array.end()) {
    return true;
  } else {
    return false;
  }
}

int main() {
  std::cout << "1. Testing file reading..." << std::endl;
  jino::Data params;
  jino::JsonReader jsonReader;
  jsonReader.readParams(params);

  std::cout << "2. Validating read data..." << std::endl;
  assert(params.size() == jino::consts::kParamNames.size());

  params.forEachDatum([&](const std::string& name, jino::DatumBase* const datum) {
    assert(params.contains(name) == true);
    assert(isStringInArray(name, jino::consts::kParamNames));
  });

  std::cout << "3. Testing data retrieval..." << std::endl;
  static_cast<void>(params.getValue<std::uint64_t>(jino::consts::kMaxTimeStep));
  const std::uint64_t samplingRate = params.getValue<std::uint64_t>(jino::consts::kSamplingRate);

  static_cast<void>(params.getValue<float>(jino::consts::kYMin));
  static_cast<void>(params.getValue<float>(jino::consts::kYMax));

  std::cout << "4. Testing element erasure..." << std::endl;
  params.erase(jino::consts::kSamplingRate);
  assert(params.contains(jino::consts::kSamplingRate) == false);
  assert(params.size() == jino::consts::kParamNames.size() - 1);

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
