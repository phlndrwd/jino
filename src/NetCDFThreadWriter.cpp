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

#include "NetCDFThreadWriter.h"

#include <chrono>
#include <filesystem>  /// NOLINT
#include <format>
#include <iostream>
#include <memory>
#include <string>

#include "Buffer.h"
#include "Buffers.h"
#include "Constants.h"
#include "NetCDFFile.h"

namespace {
std::string getFormattedDateStr() {
  auto now = std::chrono::system_clock::now();
  auto nowSeconds = floor<std::chrono::seconds>(now);
  return std::format(jino::consts::kDateFormat, nowSeconds);
}
}

jino::NetCDFThreadWriter::NetCDFThreadWriter() :  NetCDFWriterBase(), writerPool_(1) {
  std::filesystem::path path = init();
  writerPool_.enqueue([this, path] {   // Takes copy of local path variable
    try {
      file_ = std::make_unique<NetCDFFile>(path);
    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
    }
  });
}

void jino::NetCDFThreadWriter::writeMetadata(const NetCDFData& netCDFData) {
  writerPool_.enqueue([this, netCDFData] {
    writeDims(netCDFData);
    writeAttrs(netCDFData);
    writeVars(netCDFData);
  });
}

void jino::NetCDFThreadWriter::writeDatums(const NetCDFData& netCDFData) {
  writerPool_.enqueue([this, netCDFData] {
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
  });
}

void jino::NetCDFThreadWriter::writeData(const NetCDFData& netCDFData) {
  writerPool_.enqueue([this, netCDFData] {
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
  });
}

void jino::NetCDFThreadWriter::toFile(const NetCDFData& netCDFData) {
  writerPool_.enqueue([this, netCDFData] {
    writeDims(netCDFData);
    writeAttrs(netCDFData);
    writeData(netCDFData);
  });
  closeFile();
}

void jino::NetCDFThreadWriter::closeFile() {
  writerPool_.enqueue([this] {
    getFile().close();
    file_.reset();
  });
  writerPool_.waitForCompletion();
}
