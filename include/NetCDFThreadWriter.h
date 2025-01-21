/**********************************************************************************************
* Jino (JSON In NetCDF Out).                                                                  *
*                                                                                             *
* (C) Copyright 2025, Phil Underwood. This file is part of Jino.                              *
*                                                                                             *
* Jino is free software: you can redistribute it and/or modify it under the terms of the GNU  *
* Lesser General Public License as published by the Free Software Foundation, either version  *
* 3 of the License, or (at your option) any later version.                                    *
*                                                                                             *
* Jino is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without   *
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the   *
* GNU Lesser General Public License for more details.                                         *
*                                                                                             *
* You should have received a copy of the GNU General Public License along with Jino. If not,  *
* see <https://www.gnu.org/licenses/>.                                                        *
**********************************************************************************************/

#ifndef INCLUDE_NETCDFTHREADWRITER_H_
#define INCLUDE_NETCDFTHREADWRITER_H_

#include <filesystem>  /// NOLINT
#include <memory>

#include "NetCDFData.h"
#include "NetCDFFile.h"
#include "ThreadPool.h"

namespace jino {
class NetCDFThreadWriter {
 public:
  NetCDFThreadWriter();

  void init() const;
  void openFile();

  void writeMetadata(const NetCDFData&);
  void writeData(const NetCDFData&);

  void closeFile();

  const std::string& getDate() const;
  const std::filesystem::path& getPath() const;

 private:
  void writeAttrs(const NetCDFData&);
  void writeDims(const NetCDFData&);
  void writeVars(const NetCDFData&);
  void writeDatums(const NetCDFData&);

  NetCDFFile& getFile() const;

  const std::string date_;
  std::filesystem::path path_;

  ThreadPool writerPool_;

  std::unique_ptr<NetCDFFile> file_;
};
}

#endif // INCLUDE_NETCDFTHREADWRITER_H_
