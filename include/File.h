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

#ifndef INCLUDE_FILE_H_
#define INCLUDE_FILE_H_

#include <netcdf>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

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

  void addDimension(const std::string&, const std::uint64_t);
  void addVariable(const std::string&, const std::string&, const std::string&);

  template <typename T>
  void addAttribute(const std::string&, const T);

  template <typename T>
  void addData(const std::string&, const std::vector<T>&);

  void close();

 private:
  std::unique_ptr<netCDF::NcFile> netCDF_;

  std::string path_;
  netCDF::NcFile::FileMode mode_;
};
}  // namespace monio

#endif  // INCLUDE_FILE_H_

