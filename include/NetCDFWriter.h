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

#include <filesystem>
#include <memory>
#include <string>

#include "BufferBase.h"
#include "NetCDFData.h"
#include "NetCDFFile.h"

namespace jino {
class Buffers;

class NetCDFWriter {
 public:
  NetCDFWriter(const std::string&);

  void init();

  void writeMetadata(const NetCDFData&);
  void writeDatums(const NetCDFData&);
  void writeData(const NetCDFData&);
  void toFile(const NetCDFData&);

  void closeFile();

 private:
  void writeAttrs(const NetCDFData&);
  void writeDims(const NetCDFData&);
  void writeVars(const NetCDFData&);

  void writeGroupedDatum(const std::string&, const std::string&, NetCDFFile&, BufferBase* const);
  void writeUngroupedDatum(const std::string&, NetCDFFile&, BufferBase* const);

  void writeGroupedData(const std::string&, const std::string&, NetCDFFile&, BufferBase* const);
  void writeUngroupedData(const std::string&, NetCDFFile&, BufferBase* const);

  NetCDFFile& getFile();

  const std::string& date_;
  std::unique_ptr<NetCDFFile> file_;
};
}  // namespace jino

#endif // INCLUDE_NETCDFWRITER_H_
