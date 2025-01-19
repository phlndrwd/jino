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

jino::NetCDFThreadWriter::NetCDFThreadWriter() : date_(getFormattedDateStr()),
    path_(consts::kOutputDir + date_ + ".nc"), writerPool_(1) {
  init();
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
  file_ = std::make_unique<NetCDFFile>(path_);
}

void jino::NetCDFThreadWriter::openFileThread() {
  writerPool_.enqueue([this] {
    file_ = std::make_unique<NetCDFFile>(path_);
  });
}

void jino::NetCDFThreadWriter::toFile(const NetCDFData& netCDFData) {
  writeDims(netCDFData);
  writeAttrs(netCDFData);
  writeData(netCDFData);
  closeFile();
}

void jino::NetCDFThreadWriter::metadata(const NetCDFData& netCDFData) {
  writeDims(netCDFData);
  writeAttrs(netCDFData);
  writeVars(netCDFData);
}

void jino::NetCDFThreadWriter::dataThread(const NetCDFData& netCDFData) {
  writerPool_.enqueue([this, netCDFData] {
    writeDatumThread(netCDFData);
  });
}

void jino::NetCDFThreadWriter::closeFile() {
  file_->close();
}

void jino::NetCDFThreadWriter::closeFileThread() {
  writerPool_.enqueue([this] {
    file_->close();
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
      file_->addDimension(dim.name, size);
    } else {
      file_->addDimension(dim.name, NC_UNLIMITED);
    }
  });
}

void jino::NetCDFThreadWriter::writeAttrs(const NetCDFData& netCDFData) {
  for (const auto& data : netCDFData.getData()) {
    data->forEachDatum([&](const std::string& key, DatumBase* const datum) {
      switch (datum->getType()) {
        case consts::eInt8: {
          auto typedDatum = static_cast<Datum<std::int8_t>*>(datum);
          file_->addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eInt16: {
          auto typedDatum = static_cast<Datum<std::int16_t>*>(datum);
          file_->addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eInt32: {
          auto typedDatum = static_cast<Datum<std::int32_t>*>(datum);
          file_->addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eInt64: {
          auto typedDatum = static_cast<Datum<std::int64_t>*>(datum);
          file_->addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eUInt8: {
          auto typedDatum = static_cast<Datum<std::uint8_t>*>(datum);
          file_->addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eUInt16: {
          auto typedDatum = static_cast<Datum<std::uint16_t>*>(datum);
          file_->addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eUInt32: {
          auto typedDatum = static_cast<Datum<std::uint32_t>*>(datum);
          file_->addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eUInt64: {
          auto typedDatum = static_cast<Datum<std::uint64_t>*>(datum);
          file_->addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eFloat: {
          auto typedDatum = static_cast<Datum<float>*>(datum);
          file_->addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eDouble: {
          auto typedDatum = static_cast<Datum<double>*>(datum);
          file_->addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eString: {
          auto typedDatum = static_cast<Datum<std::string>*>(datum);
          file_->addAttribute(key, typedDatum->getValue());
          break;
        }
      }
    });
  }
}

void jino::NetCDFThreadWriter::writeData(const NetCDFData& netCDFData) {
  Buffers::get().forEachBuffer([&](const std::string& name, BufferBase* const buffer) {
    std::cout << "Buffer name: " << name << std::endl;
    if (buffer != nullptr) {
      std::string dimName = netCDFData.getDimensionName(buffer->size());
      file_->addVariable(name, consts::kDataTypeNames[buffer->getType()], dimName);
      switch (buffer->getType()) {
        case consts::eInt8: {
          auto typedBuffer = static_cast<Buffer<std::int8_t>*>(buffer);
          file_->addData<std::int8_t>(name, typedBuffer->getData());
          break;
        }
        case consts::eInt16: {
          auto typedBuffer = static_cast<Buffer<std::int16_t>*>(buffer);
          file_->addData<std::int16_t>(name, typedBuffer->getData());
          break;
        }
        case consts::eInt32: {
          auto typedBuffer = static_cast<Buffer<std::int32_t>*>(buffer);
          file_->addData<std::int32_t>(name, typedBuffer->getData());
          break;
        }
        case consts::eInt64: {
          auto typedBuffer = static_cast<Buffer<std::int64_t>*>(buffer);
          file_->addData<std::int64_t>(name, typedBuffer->getData());
          break;
        }
        case consts::eUInt8: {
          auto typedBuffer = static_cast<Buffer<std::uint8_t>*>(buffer);
          file_->addData<std::uint8_t>(name, typedBuffer->getData());
          break;
        }
        case consts::eUInt16: {
          auto typedBuffer = static_cast<Buffer<std::uint16_t>*>(buffer);
          file_->addData<std::uint16_t>(name, typedBuffer->getData());
          break;
        }
        case consts::eUInt32: {
          auto typedBuffer = static_cast<Buffer<std::uint32_t>*>(buffer);
          file_->addData<std::uint32_t>(name, typedBuffer->getData());
          break;
        }
        case consts::eUInt64: {
          auto typedBuffer = static_cast<Buffer<std::uint64_t>*>(buffer);
          file_->addData<std::uint64_t>(name, typedBuffer->getData());
          break;
        }
        case consts::eFloat: {
          auto typedBuffer = static_cast<Buffer<float>*>(buffer);
          file_->addData<float>(name, typedBuffer->getData());
          break;
        }
        case consts::eDouble: {
          auto typedBuffer = static_cast<Buffer<double>*>(buffer);
          file_->addData<double>(name, typedBuffer->getData());
          break;
        }
        case consts::eString: {
          auto typedBuffer = static_cast<Buffer<std::string>*>(buffer);
          file_->addData<std::string>(name, typedBuffer->getData());
          break;
        }
      }
    }
  });
}

void jino::NetCDFThreadWriter::writeVars(const NetCDFData& netCDFData) {
  Buffers::get().forEachBuffer([&](const std::string& name, BufferBase* const buffer) {
    std::cout << "Buffer name: " << name << std::endl;
    if (buffer != nullptr) {
      std::string dimName = netCDFData.getDimensionName(buffer->size());
      file_->addVariable(name, consts::kDataTypeNames[buffer->getType()], dimName);
    }
  });
}

void jino::NetCDFThreadWriter::writeDatumThread(const NetCDFData& netCDFData) {
  Buffers::get().forEachBuffer([&](const std::string& name, BufferBase* const buffer) {
    if (buffer != nullptr) {
      std::string dimName = netCDFData.getDimensionName(buffer->size());
      std::uint64_t index = buffer->getReadIndex();
      switch (buffer->getType()) {
        case consts::eInt8: {
          auto typedBuffer = static_cast<Buffer<std::int8_t>*>(buffer);
          file_->addDatum<std::int8_t>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eInt16: {
          auto typedBuffer = static_cast<Buffer<std::int16_t>*>(buffer);
          file_->addDatum<std::int16_t>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eInt32: {
          auto typedBuffer = static_cast<Buffer<std::int32_t>*>(buffer);
          file_->addDatum<std::int32_t>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eInt64: {
          auto typedBuffer = static_cast<Buffer<std::int64_t>*>(buffer);
          file_->addDatum<std::int64_t>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eUInt8: {
          auto typedBuffer = static_cast<Buffer<std::uint8_t>*>(buffer);
          file_->addDatum<std::uint8_t>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eUInt16: {
          auto typedBuffer = static_cast<Buffer<std::uint16_t>*>(buffer);
          file_->addDatum<std::uint16_t>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eUInt32: {
          auto typedBuffer = static_cast<Buffer<std::uint32_t>*>(buffer);
          file_->addDatum<std::uint32_t>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eUInt64: {
          auto typedBuffer = static_cast<Buffer<std::uint64_t>*>(buffer);
          file_->addDatum<std::uint64_t>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eFloat: {
          auto typedBuffer = static_cast<Buffer<float>*>(buffer);
          file_->addDatum<float>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eDouble: {
          auto typedBuffer = static_cast<Buffer<double>*>(buffer);
          file_->addDatum<double>(name, index, typedBuffer->getNext());
          break;
        }
        case consts::eString: {
          auto typedBuffer = static_cast<Buffer<std::string>*>(buffer);
          file_->addDatum<std::string>(name, index, typedBuffer->getNext());
          break;
        }
      }
    }
  });
}
