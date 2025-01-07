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

#include "File.h"

#include <netcdf>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

jino::File::File(const std::string& path, const netCDF::NcFile::FileMode mode) :
                 path_(path), mode_(mode) {
  try {
    file_ = std::make_unique<netCDF::NcFile>(path_, mode_);
  } catch (netCDF::exceptions::NcException& error) {
    close();
    std::cout << "ERROR: Could not access file \"" << path_ << "\"..." << std::endl;
    std::cerr << error.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

jino::File::~File() {
  close();
}

void jino::File::addDimension(const std::string& name, const std::uint64_t size) {
  file_->addDim(name, size);
}

void jino::File::addVariable(const std::string& name, const std::string& typeName,
                             const std::string& dimName) {
  file_->addVar(name, typeName, dimName);
}

template <>
void jino::File::addAttribute(const std::string& name, const std::int8_t attr) {
  file_->putAtt(name, netCDF::NcType::nc_BYTE, attr);
}

template <>
void jino::File::addAttribute(const std::string& name, const std::int16_t attr) {
  file_->putAtt(name, netCDF::NcType::nc_SHORT, attr);
}

template <>
void jino::File::addAttribute(const std::string& name, const std::int32_t attr) {
  file_->putAtt(name, netCDF::NcType::nc_INT, attr);
}

template <>
void jino::File::addAttribute(const std::string& name, const std::int64_t attr) {
  file_->putAtt(name, netCDF::NcType::nc_INT64, static_cast<unsigned long long>(attr));
}

template <>
void jino::File::addAttribute(const std::string& name, const std::uint8_t attr) {
  file_->putAtt(name, netCDF::NcType::nc_UBYTE, attr);
}

template <>
void jino::File::addAttribute(const std::string& name, const std::uint16_t attr) {
  file_->putAtt(name, netCDF::NcType::nc_USHORT, attr);
}

template <>
void jino::File::addAttribute(const std::string& name, const std::uint32_t attr) {
  file_->putAtt(name, netCDF::NcType::nc_UINT, attr);
}

template <>
void jino::File::addAttribute(const std::string& name, const std::uint64_t attr) {
  file_->putAtt(name, netCDF::NcType::nc_UINT64, static_cast<unsigned long long>(attr));
}

template <>
void jino::File::addAttribute(const std::string& name, const float attr) {
  file_->putAtt(name,  netCDF::NcType::nc_FLOAT, attr);
}

template <>
void jino::File::addAttribute(const std::string& name, const double attr) {
  file_->putAtt(name,  netCDF::NcType::nc_DOUBLE, attr);
}

template <>
void jino::File::addAttribute(const std::string& name, const long double attr) {
  file_->putAtt(name,  netCDF::NcType::nc_DOUBLE, static_cast<double>(attr));
}

template <>
void jino::File::addAttribute(const std::string& name, const std::string attr) {
  file_->putAtt(name, attr);
}

template <typename T>
void jino::File::addData(const std::string& name, const std::vector<T>& data) {
  auto var = file_->getVar(name);
  var.putVar(data.data());

  file_->putAtt("TEST", netCDF::NcType::nc_INT, 2);

            file_->putAtt("globAttrName", "globAttrStr->getValue()");
}

template void jino::File::addData<std::int8_t>(const std::string&,
              const std::vector<std::int8_t>&);
template void jino::File::addData<std::int16_t>(const std::string&,
              const std::vector<std::int16_t>&);
template void jino::File::addData<std::int32_t>(const std::string&,
              const std::vector<std::int32_t>&);
template void jino::File::addData<std::int64_t>(const std::string&,
              const std::vector<std::int64_t>&);
template void jino::File::addData<std::uint8_t>(const std::string&,
              const std::vector<std::uint8_t>&);
template void jino::File::addData<std::uint16_t>(const std::string&,
              const std::vector<std::uint16_t>&);
template void jino::File::addData<std::uint32_t>(const std::string&,
              const std::vector<std::uint32_t>&);
template void jino::File::addData<std::uint64_t>(const std::string&,
              const std::vector<std::uint64_t>&);
template void jino::File::addData<float>(const std::string&,
              const std::vector<float>&);
template void jino::File::addData<double>(const std::string&,
              const std::vector<double>&);
template void jino::File::addData<long double>(const std::string&,
              const std::vector<long double>&);
template void jino::File::addData<std::string>(const std::string&,
              const std::vector<std::string>&);

void jino::File::close() {
  file_->close();
  file_.reset();
}
