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

#ifndef INCLUDE_NETCDFTHREADWRITER_H_
#define INCLUDE_NETCDFTHREADWRITER_H_

#include "NetCDFData.h"
#include "NetCDFWriterBase.h"
#include "ThreadPool.h"

namespace jino {
class NetCDFThreadWriter : public NetCDFWriterBase {
 public:
  NetCDFThreadWriter();

  void writeMetadata(const NetCDFData&) override;
  void writeDatums(const NetCDFData&) override;
  void writeData(const NetCDFData&) override;
  void toFile(const NetCDFData&) override;

  void closeFile() override;

 private:
  ThreadPool writerPool_;
};
}

#endif // INCLUDE_NETCDFTHREADWRITER_H_
