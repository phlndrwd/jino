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

#ifndef INCLUDE_NETCDFWRITER_H_
#define INCLUDE_NETCDFWRITER_H_

#include <filesystem>  /// NOLINT
#include <memory>

#include "NetCDFData.h"
#include "NetCDFFile.h"

namespace jino {
class Buffers;

class NetCDFWriter {
 public:
  NetCDFWriter();

  void init() const;
  void openFile();

  void toFile(const NetCDFData&);
  void closeFile();

  const std::string& getDate() const;
  const std::filesystem::path& getPath() const;

 private:
  void writeAttrs(const NetCDFData&);
  void writeDims(const NetCDFData&);
  void writeData(const NetCDFData&);

  NetCDFFile& getFile() const;

  const std::string date_;
  std::filesystem::path path_;

  std::unique_ptr<NetCDFFile> file_;
};
}  // namespace jino

#endif // INCLUDE_NETCDFWRITER_H_
