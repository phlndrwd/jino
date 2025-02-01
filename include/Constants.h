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

#ifndef INCLUDE_CONSTANTS_H_
#define INCLUDE_CONSTANTS_H_

#include <array>
#include <cstdint>
#include <string>

namespace jino {
namespace consts {
enum eParams : std::uint8_t {
  eMaxTimeSteps,
  eSamplingRate,
  eWriteState,
  eYMin,
  eYMax,
  eNumberOfParams
};

enum eDataTypes : std::uint8_t {
  eInt8,
  eInt16,
  eInt32,
  eInt64,
  eUInt8,
  eUInt16,
  eUInt32,
  eUInt64,
  eFloat,
  eDouble,
  eString,
  eNumberOfDataTypes
};

enum eWriterThreads : std::uint8_t {
  eSingleThread,
  eMultiThread
};

const std::size_t kMaxFileSizeInBytes = 1048576;  // 1MB
const std::size_t kJsonIndentSize = 2;

// Other strings
constexpr std::string kSeparator = ", ";
constexpr std::string kEmptyString = "";
constexpr std::string kInputDir = "./input/";
constexpr std::string kOutputDir = "./output/";
constexpr std::string kParamsFile = "params.json";
constexpr std::string kAttrsFile = "attrs.json";
constexpr std::string kStateFile = "state.json";
constexpr std::string kJSONExtension = ".json";
constexpr std::string kNCExtension = ".nc";
;

// Parameter names
constexpr std::string kDateKey = "date";
constexpr std::string kMaxTimeStep = "MaxTimeStep";
constexpr std::string kSamplingRate = "SamplingRate";
constexpr std::string kWriteState = "WriteState";
constexpr std::string kYMin = "YMin";
constexpr std::string kYMax = "YMax";

constexpr std::string_view kDateFormat = "%Y-%m-%d_%H:%M:%S";

const std::array<std::string, eNumberOfDataTypes> kDataTypeNames = {
  "byte",
  "short",
  "int",
  "int64",
  "ubyte",
  "ushort",
  "uint",
  "uint64",
  "float",
  "double",
  "string"
};

const std::array<std::string, eNumberOfParams> kParamNames = {
  kMaxTimeStep,
  kSamplingRate,
  kWriteState,
  kYMin,
  kYMax
};

const std::array<std::uint8_t, eNumberOfParams> kParamTypes = {
  eUInt64,
  eUInt64,
  eFloat,
  eFloat,
  eUInt8
};
}  // namespace consts
}  // namespace jino

#endif  // INCLUDE_CONSTANTS_H_
