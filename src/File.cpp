/***************************************************************************************************
* JSON In NetCDF Out                                                                               *
*                                                                                                  *
* (C) Copyright 2025, Phil Underwood. All rights reserved.                                         *
*                                                                                                  *
* This program is free software: you can redistribute it and/or modify it under the terms of the   *
* GNU General Public License as published by the Free Software Foundation, either version 3 of the *
* License, or (at your option) any later version.                                                  *
*                                                                                                  *
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;        *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See    *
* the GNU General Public License for more details.                                                 *
*                                                                                                  *
* You should have received a copy of the GNU General Public License along with this program. If    *
* not, see <https://www.gnu.org/licenses/>.                                                        *
***************************************************************************************************/

#include "File.h"

#include <netcdf>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

jino::File::File(const std::string& path, const netCDF::NcFile::FileMode mode) :
                 path_(path), mode_(mode) {
  try {
    file_ = std::make_unique<netCDF::NcFile>(path_, mode_);
  } catch (netCDF::exceptions::NcException& error) {
    close();
    std::cout << "ERROR: Could not access file \"" << path_ << "\"..." << std::endl;
    std::cerr << error.what() << std::endl;
  }
}

jino::File::~File() {
  close();
}

void jino::File::close() {
  getFile().close();
  file_.reset();
}

netCDF::NcFile& jino::File::getFile() {
  if (file_ == nullptr) {
    throw std::runtime_error("ERROR: Data file has not been initialised...");
  }
  return *file_;
}
