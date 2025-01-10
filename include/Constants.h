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
  eLongDouble,
  eString,
  eNumberOfDataTypes
};

const std::size_t kMaxFileSizeInBytes = 1048576;  // 1MB

// Other strings
constexpr std::string kSeparator = ", ";
constexpr std::string kInputPath = "./input/";
constexpr std::string kParamsFile = "params.json";
constexpr std::string kAttrsFile = "attrs.json";

// Parameter names
constexpr std::string kDateKey = "date";
constexpr std::string kMaxTimeStep = "MaxTimeStep";
constexpr std::string kSamplingRate = "SamplingRate";
constexpr std::string kYMin = "YMin";
constexpr std::string kYMax = "YMax";

constexpr std::string_view kDateFormat = "{:%Y-%m-%d_%H:%M:%S}";

const std::array<std::string, eNumberOfDataTypes> kDataTypeNames = {
  "byte",
  "char",
  "short",
  "int",
  "float",
  "double",
  "unsigned byte",
  "unsigned short",
  "unsigned int",
  "long int",
  "std::string"
};

const std::array<std::string, eNumberOfParams> kParamNames = {
  kMaxTimeStep,
  kSamplingRate,
  kYMin,
  kYMax
};

const std::array<std::uint8_t, eNumberOfParams> kParamTypes = {
  eUInt64,
  eUInt64,
  eFloat,
  eFloat
};
}  // namespace consts
}  // namespace jino

#endif  // INCLUDE_CONSTANTS_H_
