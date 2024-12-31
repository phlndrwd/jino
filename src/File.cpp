/******************************************************************************
* JSON In NetCDF Out                                                          *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

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
