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

#include "NetCDFFile.h"

#include <netcdf>

#include <algorithm>
#include <string>
#include <vector>

jino::NetCDFFile::NetCDFFile(const std::filesystem::path& path,
                             const netCDF::NcFile::FileMode mode) :
                 path_(path), mode_(mode), netCDF_(path_, mode_) {}

jino::NetCDFFile::NetCDFFile(const std::filesystem::path& path) :
                 path_(path), mode_(netCDF::NcFile::replace), netCDF_(path_, mode_) {}

jino::NetCDFFile::~NetCDFFile() {
  close();
}

void jino::NetCDFFile::addDimension(const std::string& name, const std::uint64_t size) {
  if (size != 0) {
    netCDF_.addDim(name, size);
  } else {
    netCDF_.addDim(name, NC_UNLIMITED);
  }
}

void jino::NetCDFFile::addVariable(const std::string& name, const std::string& typeName,
                             const std::string& dimName) {
  netCDF_.addVar(name, typeName, dimName);
}

template <>
void jino::NetCDFFile::addAttribute(const std::string& name, const std::int8_t attr) {
  netCDF_.putAtt(name, netCDF::NcType::nc_BYTE, attr);
}

template <>
void jino::NetCDFFile::addAttribute(const std::string& name, const std::int16_t attr) {
  netCDF_.putAtt(name, netCDF::NcType::nc_SHORT, attr);
}

template <>
void jino::NetCDFFile::addAttribute(const std::string& name, const std::int32_t attr) {
  netCDF_.putAtt(name, netCDF::NcType::nc_INT, attr);
}

template <>
void jino::NetCDFFile::addAttribute(const std::string& name, const std::int64_t attr) {
  netCDF_.putAtt(name, netCDF::NcType::nc_INT64, attr);
}

template <>
void jino::NetCDFFile::addAttribute(const std::string& name, const std::uint8_t attr) {
  netCDF_.putAtt(name, netCDF::NcType::nc_UBYTE, attr);
}

template <>
void jino::NetCDFFile::addAttribute(const std::string& name, const std::uint16_t attr) {
  netCDF_.putAtt(name, netCDF::NcType::nc_USHORT, attr);
}

template <>
void jino::NetCDFFile::addAttribute(const std::string& name, const std::uint32_t attr) {
  netCDF_.putAtt(name, netCDF::NcType::nc_UINT, attr);
}

template <>
void jino::NetCDFFile::addAttribute(const std::string& name, const std::uint64_t attr) {
  /// NOLINTNEXTLINE(runtime/int)
  netCDF_.putAtt(name, netCDF::NcType::nc_UINT64, static_cast<unsigned long long>(attr));
}

template <>
void jino::NetCDFFile::addAttribute(const std::string& name, const float attr) {
  netCDF_.putAtt(name,  netCDF::NcType::nc_FLOAT, attr);
}

template <>
void jino::NetCDFFile::addAttribute(const std::string& name, const double attr) {
  netCDF_.putAtt(name,  netCDF::NcType::nc_DOUBLE, attr);
}

template <>
void jino::NetCDFFile::addAttribute(const std::string& name, const std::string attr) {
  netCDF_.putAtt(name, attr);
}

template<> void jino::NetCDFFile::addData<std::uint64_t>(const std::string& name,
                                                   const std::vector<std::uint64_t> &data) {
  std::vector<unsigned long long> castedData(data.size());  /// NOLINT(runtime/int)
  std::transform(data.begin(), data.end(), castedData.begin(), [](std::uint64_t value) {
    return static_cast<unsigned long long>(value);  /// NOLINT(runtime/int)
  });
  auto var = netCDF_.getVar(name);
  var.putVar(castedData.data());
}

template <typename T>
void jino::NetCDFFile::addData(const std::string& name, const std::vector<T>& data) {
  auto var = netCDF_.getVar(name);
  var.putVar(data.data());
}

template void jino::NetCDFFile::addData<std::int8_t>(const std::string&,
              const std::vector<std::int8_t>&);
template void jino::NetCDFFile::addData<std::int16_t>(const std::string&,
              const std::vector<std::int16_t>&);
template void jino::NetCDFFile::addData<std::int32_t>(const std::string&,
              const std::vector<std::int32_t>&);
template void jino::NetCDFFile::addData<std::int64_t>(const std::string&,
              const std::vector<std::int64_t>&);
template void jino::NetCDFFile::addData<std::uint8_t>(const std::string&,
              const std::vector<std::uint8_t>&);
template void jino::NetCDFFile::addData<std::uint16_t>(const std::string&,
              const std::vector<std::uint16_t>&);
template void jino::NetCDFFile::addData<std::uint32_t>(const std::string&,
              const std::vector<std::uint32_t>&);
template void jino::NetCDFFile::addData<float>(const std::string&,
              const std::vector<float>&);
template void jino::NetCDFFile::addData<double>(const std::string&,
              const std::vector<double>&);
template void jino::NetCDFFile::addData<std::string>(const std::string&,
              const std::vector<std::string>&);

template <typename T>
void jino::NetCDFFile::addDatum(const std::string& name, const std::uint64_t index, const T datum) {
  auto var = netCDF_.getVar(name);
  std::vector<uint64_t> indexVec = {index};
  var.putVar(indexVec, datum);
  netCDF_.sync();
}

template void jino::NetCDFFile::addDatum<std::int8_t>(const std::string&, const std::uint64_t,
                                                      const std::int8_t);
template void jino::NetCDFFile::addDatum<std::int16_t>(const std::string&, const std::uint64_t,
                                                       const std::int16_t);
template void jino::NetCDFFile::addDatum<std::int32_t>(const std::string&, const std::uint64_t,
                                                       const std::int32_t);
template void jino::NetCDFFile::addDatum<std::int64_t>(const std::string&, const std::uint64_t,
                                                       const std::int64_t);
template void jino::NetCDFFile::addDatum<std::uint8_t>(const std::string&, const std::uint64_t,
                                                       const std::uint8_t);
template void jino::NetCDFFile::addDatum<std::uint16_t>(const std::string&, const std::uint64_t,
                                                        const std::uint16_t);
template void jino::NetCDFFile::addDatum<std::uint32_t>(const std::string&, const std::uint64_t,
                                                        const std::uint32_t);
template void jino::NetCDFFile::addDatum<float>(const std::string&, const std::uint64_t,
                                                const float);
template void jino::NetCDFFile::addDatum<double>(const std::string&, const std::uint64_t,
                                                 const double);
template void jino::NetCDFFile::addDatum<std::string>(const std::string&, const std::uint64_t,
                                                      const std::string);

template <>
void jino::NetCDFFile::addDatum(const std::string& name, const std::uint64_t index,
                                const std::uint64_t datum) {
  auto var = netCDF_.getVar(name);
  std::vector<uint64_t> indexVec = {index};
  var.putVar(indexVec, static_cast<unsigned long long>(datum));  /// NOLINT(runtime/int)
}


void jino::NetCDFFile::close() {
  netCDF_.close();
}
