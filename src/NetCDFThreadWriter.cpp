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
#include "Data.h"
#include "Datum.h"
#include "NetCDFDim.h"
#include "NetCDFFile.h"

namespace {
std::string getFormattedDateStr() {
  auto now = std::chrono::system_clock::now();
  auto nowSeconds = floor<std::chrono::seconds>(now);
  return std::format(jino::consts::kDateFormat, nowSeconds);
}
}

jino::NetCDFThreadWriter::NetCDFThreadWriter() :
        date_(getFormattedDateStr()), path_(consts::kOutputDir + date_ + ".nc"), writerPool_(1) {
  init();
  openFile();
}

void jino::NetCDFThreadWriter::init() const {
  const std::filesystem::path dir = std::filesystem::path(path_).parent_path();
  try {
    if (!std::filesystem::exists(dir)) {
      std::filesystem::create_directories(dir);
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

void jino::NetCDFThreadWriter::openFile() {
  writerPool_.enqueue([this] {
    file_ = std::make_unique<NetCDFFile>(path_);
  });
}

void jino::NetCDFThreadWriter::writeMetadata(const NetCDFData& netCDFData) {
  writerPool_.enqueue([this, netCDFData] {
    writeDims(netCDFData);
  });
  writerPool_.enqueue([this, netCDFData] {
    writeAttrs(netCDFData);
  });
  writerPool_.enqueue([this, netCDFData] {
    writeVars(netCDFData);
  });
}

void jino::NetCDFThreadWriter::writeData(const NetCDFData& netCDFData) {
  writerPool_.enqueue([this, netCDFData] {
    writeDatum(netCDFData);
  });
}

void jino::NetCDFThreadWriter::waitForCompletion() {
  writerPool_.waitForCompletion();
}

void jino::NetCDFThreadWriter::closeFile() {
  writerPool_.enqueue([this] {
    getFile().close();
    file_.reset();
  });
}

const std::string& jino::NetCDFThreadWriter::getDate() const {
  return date_;
}

const std::string& jino::NetCDFThreadWriter::getPath() const {
  return path_;
}

void jino::NetCDFThreadWriter::writeDims(const NetCDFData& netCDFData) {
  netCDFData.forEachDimension([&](const NetCDFDim& dim, const std::uint64_t size) {
    if (dim.isUnlimited == false) {
      getFile().addDimension(dim.name, size);
    } else {
      getFile().addDimension(dim.name, NC_UNLIMITED);
    }
  });
}

void jino::NetCDFThreadWriter::writeAttrs(const NetCDFData& netCDFData) {
  for (const auto& data : netCDFData.getData()) {
    data->forEachDatum([&](const std::string& key, DatumBase* const datum) {
      switch (datum->getType()) {
        case consts::eInt8: {
          auto typedDatum = static_cast<Datum<std::int8_t>*>(datum);
          getFile().addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eInt16: {
          auto typedDatum = static_cast<Datum<std::int16_t>*>(datum);
          getFile().addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eInt32: {
          auto typedDatum = static_cast<Datum<std::int32_t>*>(datum);
          getFile().addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eInt64: {
          auto typedDatum = static_cast<Datum<std::int64_t>*>(datum);
          getFile().addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eUInt8: {
          auto typedDatum = static_cast<Datum<std::uint8_t>*>(datum);
          getFile().addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eUInt16: {
          auto typedDatum = static_cast<Datum<std::uint16_t>*>(datum);
          getFile().addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eUInt32: {
          auto typedDatum = static_cast<Datum<std::uint32_t>*>(datum);
          getFile().addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eUInt64: {
          auto typedDatum = static_cast<Datum<std::uint64_t>*>(datum);
          getFile().addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eFloat: {
          auto typedDatum = static_cast<Datum<float>*>(datum);
          getFile().addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eDouble: {
          auto typedDatum = static_cast<Datum<double>*>(datum);
          getFile().addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eString: {
          auto typedDatum = static_cast<Datum<std::string>*>(datum);
          getFile().addAttribute(key, typedDatum->getValue());
          break;
        }
      }
    });
  }
}

void jino::NetCDFThreadWriter::writeVars(const NetCDFData& netCDFData) {
  Buffers::get().forEachBuffer([&](const std::string& name, BufferBase* const buffer) {
    if (buffer != nullptr) {
      std::string dimName = netCDFData.getDimensionName(buffer->size());
      getFile().addVariable(name, consts::kDataTypeNames[buffer->getType()], dimName);
    }
  });
}

void jino::NetCDFThreadWriter::writeDatum(const NetCDFData& netCDFData) {
  Buffers::get().forEachBuffer([&](const std::string& name, BufferBase* const buffer) {
    if (buffer != nullptr) {
      std::string dimName = netCDFData.getDimensionName(buffer->size());
      std::uint64_t index = buffer->getReadIndex();
      switch (buffer->getType()) {
        case consts::eInt8: {
          auto typedBuffer = static_cast<Buffer<std::int8_t>*>(buffer);
          getFile().addDatum<std::int8_t>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eInt16: {
          auto typedBuffer = static_cast<Buffer<std::int16_t>*>(buffer);
          getFile().addDatum<std::int16_t>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eInt32: {
          auto typedBuffer = static_cast<Buffer<std::int32_t>*>(buffer);
          getFile().addDatum<std::int32_t>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eInt64: {
          auto typedBuffer = static_cast<Buffer<std::int64_t>*>(buffer);
          getFile().addDatum<std::int64_t>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eUInt8: {
          auto typedBuffer = static_cast<Buffer<std::uint8_t>*>(buffer);
          getFile().addDatum<std::uint8_t>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eUInt16: {
          auto typedBuffer = static_cast<Buffer<std::uint16_t>*>(buffer);
          getFile().addDatum<std::uint16_t>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eUInt32: {
          auto typedBuffer = static_cast<Buffer<std::uint32_t>*>(buffer);
          getFile().addDatum<std::uint32_t>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eUInt64: {
          auto typedBuffer = static_cast<Buffer<std::uint64_t>*>(buffer);
          getFile().addDatum<std::uint64_t>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eFloat: {
          auto typedBuffer = static_cast<Buffer<float>*>(buffer);
          getFile().addDatum<float>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eDouble: {
          auto typedBuffer = static_cast<Buffer<double>*>(buffer);
          getFile().addDatum<double>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eString: {
          auto typedBuffer = static_cast<Buffer<std::string>*>(buffer);
          getFile().addDatum<std::string>(name, index, typedBuffer->getNext());
          break;
        }
      }
    }
  });
}

jino::NetCDFFile& jino::NetCDFThreadWriter::getFile() const {
  if (file_ == nullptr) {
    throw std::runtime_error("ERROR: NetCDFFile has not been initialised...");
  }
  return *file_;
}
