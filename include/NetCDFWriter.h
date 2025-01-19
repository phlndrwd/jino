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

#ifndef INCLUDE_NETCDFWRITER_H_
#define INCLUDE_NETCDFWRITER_H_

#include <memory>

#include "NetCDFData.h"
#include "NetCDFFile.h"
#include "ThreadPool.h"

namespace jino {
class Buffers;

class NetCDFWriter {
 public:
  NetCDFWriter();

  void init() const;

  void openFile();
  void openFileThread();

  void toFile(const NetCDFData&);
  void metadata(const NetCDFData&);
  void dataThread(const NetCDFData&);

  void closeFile();
  void closeFileThread();

  const std::string& getDate() const;
  const std::string& getPath() const;

 private:
  void writeAttrs(const NetCDFData&);
  void writeDims(const NetCDFData&);
  void writeData(const NetCDFData&);
  void writeVars(const NetCDFData&);
  void writeDatumThread(const NetCDFData&);

  const std::string date_;
  const std::string path_;
  ThreadPool writerPool_;

  std::unique_ptr<NetCDFFile> file_;
};
}  // namespace jino

#endif // INCLUDE_NETCDFWRITER_H_
