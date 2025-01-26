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

#include "NetCDFWriterBase.h"

#include <chrono>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>

#include "Buffer.h"
#include "Buffers.h"
#include "Constants.h"
#include "Datum.h"
#include "NetCDFData.h"
#include "NetCDFDim.h"

namespace {
std::string getFormattedDateStr() {
  auto now = std::chrono::system_clock::now();
  auto nowSeconds = std::chrono::system_clock::to_time_t(now);

  std::ostringstream oss;
  std::string dateFormat = std::string(jino::consts::kDateFormat);
  oss << std::put_time(std::localtime(&nowSeconds), dateFormat.c_str());
  return oss.str();
}
}

jino::NetCDFWriterBase::NetCDFWriterBase() : date_(getFormattedDateStr()) {}

std::filesystem::path jino::NetCDFWriterBase::init() const {
  try {
    if (std::filesystem::exists(consts::kOutputDir) == false) {
      std::filesystem::create_directories(consts::kOutputDir);
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  std::uint32_t count = 1;
  std::filesystem::path path(consts::kOutputDir + date_ + ".nc");
  while (std::filesystem::exists(path) == true) {
    path = consts::kOutputDir + date_ + "(" + std::to_string(count) + ").nc";
    ++count;
  }
  return path;
}

const std::string& jino::NetCDFWriterBase::getDate() const {
  return date_;
}

void jino::NetCDFWriterBase::writeDims(const NetCDFData& netCDFData) {
  NetCDFFile& file = getFile();
  netCDFData.forEachDimension([&file](const NetCDFDim& dim, const std::uint64_t size) {
    if (dim.isUnlimited == false) {
      file.addDimension(dim.name, size);
    } else {
      file.addDimension(dim.name, NC_UNLIMITED);
    }
  });
}

void jino::NetCDFWriterBase::writeAttrs(const NetCDFData& netCDFData) {
  NetCDFFile& file = getFile();
  for (const auto& data : netCDFData.getData()) {
    data->forEachDatum([&file](const std::string& key, DatumBase* const datum) {
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

void jino::NetCDFWriterBase::writeVars(const NetCDFData& netCDFData) {
  NetCDFFile& file = getFile();
  Buffers::get().forEachBuffer([&netCDFData, &file](const BufferKey& key,
                                                    BufferBase* const buffer) {
    if (buffer != nullptr) {
      const std::string& dimName = netCDFData.getDimensionName(buffer->size());
      const std::string& groupName = key.groupName;
      if (groupName == consts::kEmptyString) {
        file.addVariable(key.varName, consts::kDataTypeNames[buffer->getType()], dimName);
      } else {
        file.addVariable(key.varName, groupName,
                         consts::kDataTypeNames[buffer->getType()], dimName);
      }
    }
  });
}

void jino::NetCDFWriterBase::writeGroupedDatum(const std::string& name,
                 const std::string& groupName, NetCDFFile& file, BufferBase* const buffer) {
  const std::uint64_t index = buffer->getReadIndex();
  switch (buffer->getType()) {
    case consts::eInt8: {
      auto typedBuffer = static_cast<Buffer<std::int8_t>*>(buffer);
      file.addDatum<std::int8_t>(name, groupName, index, typedBuffer->getNext());
      break;
    }
    case consts::eInt16: {
      auto typedBuffer = static_cast<Buffer<std::int16_t>*>(buffer);
      file.addDatum<std::int16_t>(name, groupName, index, typedBuffer->getNext());
      break;
    }
    case consts::eInt32: {
      auto typedBuffer = static_cast<Buffer<std::int32_t>*>(buffer);
      file.addDatum<std::int32_t>(name, groupName, index, typedBuffer->getNext());
      break;
    }
    case consts::eInt64: {
      auto typedBuffer = static_cast<Buffer<std::int64_t>*>(buffer);
      file.addDatum<std::int64_t>(name, groupName, index, typedBuffer->getNext());
      break;
    }
    case consts::eUInt8: {
      auto typedBuffer = static_cast<Buffer<std::uint8_t>*>(buffer);
      file.addDatum<std::uint8_t>(name, groupName, index, typedBuffer->getNext());
      break;
    }
    case consts::eUInt16: {
      auto typedBuffer = static_cast<Buffer<std::uint16_t>*>(buffer);
      file.addDatum<std::uint16_t>(name, groupName, index, typedBuffer->getNext());
      break;
    }
    case consts::eUInt32: {
      auto typedBuffer = static_cast<Buffer<std::uint32_t>*>(buffer);
      file.addDatum<std::uint32_t>(name, groupName, index, typedBuffer->getNext());
      break;
    }
    case consts::eUInt64: {
      auto typedBuffer = static_cast<Buffer<std::uint64_t>*>(buffer);
      file.addDatum<std::uint64_t>(name, groupName, index, typedBuffer->getNext());
      break;
    }
    case consts::eFloat: {
      auto typedBuffer = static_cast<Buffer<float>*>(buffer);
      file.addDatum<float>(name, groupName, index, typedBuffer->getNext());
      break;
    }
    case consts::eDouble: {
      auto typedBuffer = static_cast<Buffer<double>*>(buffer);
      file.addDatum<double>(name, groupName, index, typedBuffer->getNext());
      break;
    }
    case consts::eString: {
      auto typedBuffer = static_cast<Buffer<std::string>*>(buffer);
      file.addDatum<std::string>(name, groupName, index, typedBuffer->getNext());
      break;
    }
  }
}

void jino::NetCDFWriterBase::writeUngroupedDatum(const std::string& name, NetCDFFile& file,
                                                 BufferBase* const buffer) {
  const std::uint64_t index = buffer->getReadIndex();
  switch (buffer->getType()) {
    case consts::eInt8: {
      auto typedBuffer = static_cast<Buffer<std::int8_t>*>(buffer);
      file.addDatum<std::int8_t>(name, index, typedBuffer->getNext());
      break;
    }
    case consts::eInt16: {
      auto typedBuffer = static_cast<Buffer<std::int16_t>*>(buffer);
      file.addDatum<std::int16_t>(name, index, typedBuffer->getNext());
      break;
    }
    case consts::eInt32: {
      auto typedBuffer = static_cast<Buffer<std::int32_t>*>(buffer);
      file.addDatum<std::int32_t>(name, index, typedBuffer->getNext());
      break;
    }
    case consts::eInt64: {
      auto typedBuffer = static_cast<Buffer<std::int64_t>*>(buffer);
      file.addDatum<std::int64_t>(name, index, typedBuffer->getNext());
      break;
    }
    case consts::eUInt8: {
      auto typedBuffer = static_cast<Buffer<std::uint8_t>*>(buffer);
      file.addDatum<std::uint8_t>(name, index, typedBuffer->getNext());
      break;
    }
    case consts::eUInt16: {
      auto typedBuffer = static_cast<Buffer<std::uint16_t>*>(buffer);
      file.addDatum<std::uint16_t>(name, index, typedBuffer->getNext());
      break;
    }
    case consts::eUInt32: {
      auto typedBuffer = static_cast<Buffer<std::uint32_t>*>(buffer);
      file.addDatum<std::uint32_t>(name, index, typedBuffer->getNext());
      break;
    }
    case consts::eUInt64: {
      auto typedBuffer = static_cast<Buffer<std::uint64_t>*>(buffer);
      file.addDatum<std::uint64_t>(name, index, typedBuffer->getNext());
      break;
    }
    case consts::eFloat: {
      auto typedBuffer = static_cast<Buffer<float>*>(buffer);
      file.addDatum<float>(name, index, typedBuffer->getNext());
      break;
    }
    case consts::eDouble: {
      auto typedBuffer = static_cast<Buffer<double>*>(buffer);
      file.addDatum<double>(name, index, typedBuffer->getNext());
      break;
    }
    case consts::eString: {
      auto typedBuffer = static_cast<Buffer<std::string>*>(buffer);
      file.addDatum<std::string>(name, index, typedBuffer->getNext());
      break;
    }
  }
}
void jino::NetCDFWriterBase::writeGroupedData(const std::string& name, const std::string& groupName,
                                              NetCDFFile& file, BufferBase* const buffer) {
  switch (buffer->getType()) {
    case consts::eInt8: {
      auto typedBuffer = static_cast<Buffer<std::int8_t>*>(buffer);
      file.addData<std::int8_t>(name, groupName, typedBuffer->getData());
      break;
    }
    case consts::eInt16: {
      auto typedBuffer = static_cast<Buffer<std::int16_t>*>(buffer);
      file.addData<std::int16_t>(name, groupName, typedBuffer->getData());
      break;
    }
    case consts::eInt32: {
      auto typedBuffer = static_cast<Buffer<std::int32_t>*>(buffer);
      file.addData<std::int32_t>(name, groupName, typedBuffer->getData());
      break;
    }
    case consts::eInt64: {
      auto typedBuffer = static_cast<Buffer<std::int64_t>*>(buffer);
      file.addData<std::int64_t>(name, groupName, typedBuffer->getData());
      break;
    }
    case consts::eUInt8: {
      auto typedBuffer = static_cast<Buffer<std::uint8_t>*>(buffer);
      file.addData<std::uint8_t>(name, groupName, typedBuffer->getData());
      break;
    }
    case consts::eUInt16: {
      auto typedBuffer = static_cast<Buffer<std::uint16_t>*>(buffer);
      file.addData<std::uint16_t>(name, groupName, typedBuffer->getData());
      break;
    }
    case consts::eUInt32: {
      auto typedBuffer = static_cast<Buffer<std::uint32_t>*>(buffer);
      file.addData<std::uint32_t>(name, groupName, typedBuffer->getData());
      break;
    }
    case consts::eUInt64: {
      auto typedBuffer = static_cast<Buffer<std::uint64_t>*>(buffer);
      file.addData<std::uint64_t>(name, groupName, typedBuffer->getData());
      break;
    }
    case consts::eFloat: {
      auto typedBuffer = static_cast<Buffer<float>*>(buffer);
      file.addData<float>(name, groupName, typedBuffer->getData());
      break;
    }
    case consts::eDouble: {
      auto typedBuffer = static_cast<Buffer<double>*>(buffer);
      file.addData<double>(name, groupName, typedBuffer->getData());
      break;
    }
    case consts::eString: {
      auto typedBuffer = static_cast<Buffer<std::string>*>(buffer);
      file.addData<std::string>(name, groupName, typedBuffer->getData());
      break;
    }
  }
}

void jino::NetCDFWriterBase::writeUngroupedData(const std::string& name, NetCDFFile& file,
                                                 BufferBase* const buffer) {
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

jino::NetCDFFile& jino::NetCDFWriterBase::getFile() const {
  if (file_ == nullptr) {
    throw std::runtime_error("ERROR: NetCDFFile has not been initialised...");
  }
  return *file_;
}
