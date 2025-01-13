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

#include "NetCDFWriter.h"

#include <chrono>
#include <filesystem>  /// NOLINT
#include <format>
#include <iostream>
#include <string>

#include "Buffer.h"
#include "Constants.h"
#include "Data.h"
#include "Datum.h"
#include "NetCDFFile.h"
#include "Output.h"

namespace {
std::string getFormattedDateStr() {
  auto now = std::chrono::system_clock::now();
  auto nowSeconds = floor<std::chrono::seconds>(now);
  return std::format(jino::consts::kDateFormat, nowSeconds);
}
}


jino::NetCDFWriter::NetCDFWriter() : date_(getFormattedDateStr()),
    path_(consts::kOutputDir + date_ + ".nc") {}

void jino::NetCDFWriter::initOutput() const {
  const std::filesystem::path dir = std::filesystem::path(path_).parent_path();
  try {
    if (!std::filesystem::exists(dir)) {
      std::filesystem::create_directories(dir);
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

void jino::NetCDFWriter::toFile(NetCDFFile& file, const NetCDFData& netCDFData) const {
  writeDims(file, netCDFData);
  writeAttrs(file, netCDFData);
  writeData(file, netCDFData);
}

const std::string& jino::NetCDFWriter::getDate() const {
  return date_;
}

const std::string& jino::NetCDFWriter::getPath() const {
  return path_;
}

void jino::NetCDFWriter::writeDims(NetCDFFile& file, const NetCDFData& netCDFData) const {
  netCDFData.forEachDimension([&](const std::string& name, const std::uint64_t size) {
    file.addDimension(name, size);
  });
}

void jino::NetCDFWriter::writeAttrs(NetCDFFile& file, const NetCDFData& netCDFData) const {
  for (const auto& data : netCDFData.getData()) {
    data->forEachDatum([&](const std::string& key, DatumBase* const datum) {
      switch (datum->getType()) {
        case consts::eInt8: {
          auto typedDatum = static_cast<Datum<std::int8_t>*>(datum);
          file.addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eInt16: {
          auto typedDatum = static_cast<Datum<std::int16_t>*>(datum);
          file.addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eInt32: {
          auto typedDatum = static_cast<Datum<std::int32_t>*>(datum);
          file.addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eInt64: {
          auto typedDatum = static_cast<Datum<std::int64_t>*>(datum);
          file.addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eUInt8: {
          auto typedDatum = static_cast<Datum<std::uint8_t>*>(datum);
          file.addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eUInt16: {
          auto typedDatum = static_cast<Datum<std::uint16_t>*>(datum);
          file.addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eUInt32: {
          auto typedDatum = static_cast<Datum<std::uint32_t>*>(datum);
          file.addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eUInt64: {
          auto typedDatum = static_cast<Datum<std::uint64_t>*>(datum);
          file.addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eFloat: {
          auto typedDatum = static_cast<Datum<float>*>(datum);
          file.addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eDouble: {
          auto typedDatum = static_cast<Datum<double>*>(datum);
          file.addAttribute(key, typedDatum->getValue());
          break;
        }
        case consts::eString: {
          auto typedDatum = static_cast<Datum<std::string>*>(datum);
          file.addAttribute(key, typedDatum->getValue());
          break;
        }
      }
    });
  }
}

void jino::NetCDFWriter::writeData(NetCDFFile& file, const NetCDFData& netCDFData) const {
  Output::get().forEachBuffer([&](const std::string& name, BufferBase* const buffer) {
    std::cout << "Buffer name: " << name << std::endl;
    if (buffer != nullptr) {
      std::string dimName = netCDFData.getDimensionName(buffer->size());
      file.addVariable(name, "double", dimName);
      switch (buffer->getType()) {
        case consts::eInt8: {
          auto typedBuffer = static_cast<Buffer<std::int8_t>*>(buffer);
          file.addData<std::int8_t>(name, typedBuffer->getData());
          break;
        }
        case consts::eInt16: {
          auto typedBuffer = static_cast<Buffer<std::int16_t>*>(buffer);
          file.addData<std::int16_t>(name, typedBuffer->getData());
          break;
        }
        case consts::eInt32: {
          auto typedBuffer = static_cast<Buffer<std::int32_t>*>(buffer);
          file.addData<std::int32_t>(name, typedBuffer->getData());
          break;
        }
        case consts::eInt64: {
          auto typedBuffer = static_cast<Buffer<std::int64_t>*>(buffer);
          file.addData<std::int64_t>(name, typedBuffer->getData());
          break;
        }
        case consts::eUInt8: {
          auto typedBuffer = static_cast<Buffer<std::uint8_t>*>(buffer);
          file.addData<std::uint8_t>(name, typedBuffer->getData());
          break;
        }
        case consts::eUInt16: {
          auto typedBuffer = static_cast<Buffer<std::uint16_t>*>(buffer);
          file.addData<std::uint16_t>(name, typedBuffer->getData());
          break;
        }
        case consts::eUInt32: {
          auto typedBuffer = static_cast<Buffer<std::uint32_t>*>(buffer);
          file.addData<std::uint32_t>(name, typedBuffer->getData());
          break;
        }
        case consts::eUInt64: {
          auto typedBuffer = static_cast<Buffer<std::uint64_t>*>(buffer);
          file.addData<std::uint64_t>(name, typedBuffer->getData());
          break;
        }
        case consts::eFloat: {
          auto typedBuffer = static_cast<Buffer<float>*>(buffer);
          file.addData<float>(name, typedBuffer->getData());
          break;
        }
        case consts::eDouble: {
          auto typedBuffer = static_cast<Buffer<double>*>(buffer);
          file.addData<double>(name, typedBuffer->getData());
          break;
        }
        case consts::eString: {
          auto typedBuffer = static_cast<Buffer<std::string>*>(buffer);
          file.addData<std::string>(name, typedBuffer->getData());
          break;
        }
      }
    }
  });
}
