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

#include "NetCDFWriter.h"

#include <iostream>
#include <memory>
#include <string>

#include "Buffer.h"
#include "Buffers.h"
#include "Constants.h"
#include "NetCDFFile.h"

jino::NetCDFWriter::NetCDFWriter(const std::string& date) : NetCDFWriterBase() {
  try {
    file_ = std::make_unique<NetCDFFile>(NetCDFWriterBase::init(date));
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

void jino::NetCDFWriter::writeMetadata(const NetCDFData& netCDFData) {
  writeDims(netCDFData);
  writeAttrs(netCDFData);
  writeVars(netCDFData);
}

void jino::NetCDFWriter::writeDatums(const NetCDFData& netCDFData) {
  NetCDFFile& file = getFile();
  Buffers::get().forEachBuffer([this, &netCDFData, &file](const BufferKey& key,
                                                          BufferBase* const buffer) {
    if (buffer != nullptr) {
      const std::string& groupName = key.groupName;
      if (groupName != consts::kEmptyString) {
        writeGroupedDatum(key.varName, groupName, file, buffer);
      } else {
        writeUngroupedDatum(key.varName, file, buffer);
      }
    }
  });
}

void jino::NetCDFWriter::writeData(const NetCDFData& netCDFData) {
  NetCDFFile& file = getFile();
  Buffers::get().forEachBuffer([this, &netCDFData, &file](const BufferKey& key,
                                                          BufferBase* const buffer) {
    if (buffer != nullptr) {
      const std::string& dimName = netCDFData.getDimensionName(buffer->size());
      const std::string& groupName = key.groupName;
      if (groupName != consts::kEmptyString) {
        file.addVariable(key.varName, groupName,
                         consts::kDataTypeNames[buffer->getType()], dimName);
        writeGroupedData(key.varName, groupName, file, buffer);
      } else {
        file.addVariable(key.varName, consts::kDataTypeNames[buffer->getType()], dimName);
        writeUngroupedData(key.varName, file, buffer);
      }
    }
  });
}

void jino::NetCDFWriter::toFile(const NetCDFData& netCDFData) {
  writeDims(netCDFData);
  writeAttrs(netCDFData);
  writeData(netCDFData);
  closeFile();
}

void jino::NetCDFWriter::closeFile() {
  getFile().close();
  file_.reset();
}
