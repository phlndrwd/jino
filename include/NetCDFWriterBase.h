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

#ifndef INCLUDE_NETCDFWRITERBASE_H_
#define INCLUDE_NETCDFWRITERBASE_H_

#include <filesystem>
#include <memory>
#include <string>

#include "BufferBase.h"
#include "NetCDFData.h"
#include "NetCDFFile.h"

namespace jino {
class Buffers;

class NetCDFWriterBase {
 public:
  NetCDFWriterBase();

  inline const std::string& getDate() const {
    return date_;
  }
  std::filesystem::path init() const;

  virtual void writeMetadata(const NetCDFData&) = 0;
  virtual void writeDatums(const NetCDFData&) = 0;
  virtual void writeData(const NetCDFData&) = 0;
  virtual void toFile(const NetCDFData&) = 0;

  virtual void closeFile() = 0;

 protected:
  void writeAttrs(const NetCDFData&);
  void writeDims(const NetCDFData&);
  void writeVars(const NetCDFData&);

  void writeGroupedDatum(const std::string&, const std::string&, NetCDFFile&, BufferBase* const);
  void writeUngroupedDatum(const std::string&, NetCDFFile&, BufferBase* const);

  void writeGroupedData(const std::string&, const std::string&, NetCDFFile&, BufferBase* const);
  void writeUngroupedData(const std::string&, NetCDFFile&, BufferBase* const);

  inline NetCDFFile& getFile() const {
    if (file_ == nullptr) {
      throw std::runtime_error("ERROR: NetCDFFile has not been initialised...");
    }
    return *file_;
  }

  const std::string date_;
  std::unique_ptr<NetCDFFile> file_;
};
}  // namespace jino
#endif // INCLUDE_NETCDFWRITERBASE_H_
