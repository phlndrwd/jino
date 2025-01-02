/**********************************************************************************************
* JSON In NetCDF Out                                                                          *
*                                                                                             *
* (C) Copyright 2025, Phil Underwood. All rights reserved.                                    *
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

#include <cassert>
#include <iostream>
#include <vector>

#include "Buffer.h"
#include "Constants.h"
#include "Data.h"
#include "FileReader.h"

int main() {
  std::cout << "1. Testing file reading..." << std::endl;
  jino::Data params;
  jino::FileReader fileReader;
  fileReader.getParams(params);

  std::cout << "2. Validating read data..." << std::endl;
  assert(params.keys().size() == params.size());
  for (std::uint64_t i = 0; i < params.size(); i++) {
    assert(params.keyAt(i) == jino::consts::kParamNames.at(i));
    static_cast<void>(params[i].getValueStr());
  }

  std::cout << "3. Testing data retrieval..." << std::endl;
  static_cast<void>(params.getValue<std::uint64_t>(jino::consts::kMaxTimeSteps));
  const std::uint64_t samplingRate = params.getValue<std::uint64_t>(jino::consts::kSamplingRate);

  static_cast<void>(params.getValue<float>(jino::consts::kYMin));
  static_cast<void>(params.getValue<float>(jino::consts::kYMax));

  std::cout << "4. Testing element erasure..." << std::endl;
  params.erase(jino::consts::kSamplingRate);
  assert(params.contains(jino::consts::kSamplingRate) == false);
  assert(params.keys().size() == params.size());

  std::cout << "5. Testing buffer creation..." << std::endl;
  jino::Buffer<std::int64_t> buffer(samplingRate);
  assert(buffer.size() == samplingRate);
  for (std::uint64_t i = 0; i < samplingRate; ++i) {
    buffer.at(i) = i;
  }
  assert(buffer.at(samplingRate - 1) == samplingRate - 1);
  std::cout << "All Passed." << std::endl;
  return 0;
}
