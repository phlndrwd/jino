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

#ifndef INCLUDE_NETCDFFILE_H_
#define INCLUDE_NETCDFFILE_H_

#include <netcdf>

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace jino {
class NetCDFFile {
 public:
  explicit NetCDFFile(const std::filesystem::path&, const netCDF::NcFile::FileMode);
  explicit NetCDFFile(const std::filesystem::path&);

  ~NetCDFFile();

  NetCDFFile()                       = delete;
  NetCDFFile(NetCDFFile&&)                 = delete;
  NetCDFFile(const NetCDFFile&)            = delete;
  NetCDFFile& operator=(NetCDFFile&&)      = delete;
  NetCDFFile& operator=(const NetCDFFile&) = delete;

  void addDimension(const std::string&, const std::uint64_t);
  void addVariable(const std::string&, const std::string&, const std::string&);
  void addVariable(const std::string&, const std::string&, const std::string&, const std::string&);


  template <typename T>
  void addAttribute(const std::string&, const T);

  template <typename T>
  void addData(const std::string&, const std::vector<T>&);

  template <typename T>
  void addData(const std::string&, const std::string&, const std::vector<T>&);

  template <typename T>
  void addDatum(const std::string&, const std::uint64_t, const T);

  template <typename T>
  void addDatum(const std::string&, const std::string&, const std::uint64_t, const T);

  void close();

 private:
  const std::filesystem::path path_;
  const netCDF::NcFile::FileMode mode_;
  netCDF::NcFile netCDF_;
};
}  // namespace monio

#endif  // INCLUDE_NETCDFFILE_H_

