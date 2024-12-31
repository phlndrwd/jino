/******************************************************************************
* JSON In NetCDF Out                                                          *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef SRC_FILE_H_
#define SRC_FILE_H_

#include <netcdf>

#include <memory>
#include <string>

namespace jino {
class File {
 public:
  File(const std::string&, const netCDF::NcFile::FileMode);
  ~File();

  File()                       = delete;
  File(File&&)                 = delete;
  File(const File&)            = delete;
  File& operator=(File&&)      = delete;
  File& operator=(const File&) = delete;

  void close();

 private:
  netCDF::NcFile& getFile();

  std::unique_ptr<netCDF::NcFile> file_;

  std::string path_;
  netCDF::NcFile::FileMode mode_;
};
}  // namespace monio

#endif  // SRC_FILE_H_

