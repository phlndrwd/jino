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
#include "Datum.h"
#include "NetCDFFile.h"

jino::NetCDFWriter::NetCDFWriter(const std::string& date) : date_(date) {}

void jino::NetCDFWriter::init() {
  std::uint32_t count = 1;
  std::filesystem::path path(consts::kOutputDir + date_ + consts::kNCExtension);
  while (std::filesystem::exists(path) == true) {
    path = consts::kOutputDir + date_ + "(" + std::to_string(count) + ")" + consts::kNCExtension;
    ++count;
  }
  try {
    file_ = std::make_unique<NetCDFFile>(path);
  } catch (const std::exception& error) {
    std::cerr << error.what() << std::endl;
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

void jino::NetCDFWriter::writeDims(const NetCDFData& netCDFData) {
  NetCDFFile& file = getFile();
  netCDFData.forEachDimension([&file](const NetCDFDim& dim, const std::uint64_t size) {
    if (dim.isUnlimited == false) {
      file.addDimension(dim.name, size);
    } else {
      file.addDimension(dim.name, NC_UNLIMITED);
    }
  });
}

void jino::NetCDFWriter::writeAttrs(const NetCDFData& netCDFData) {
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

void jino::NetCDFWriter::writeVars(const NetCDFData& netCDFData) {
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

void jino::NetCDFWriter::writeGroupedDatum(const std::string& name,
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

void jino::NetCDFWriter::writeUngroupedDatum(const std::string& name, NetCDFFile& file,
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
void jino::NetCDFWriter::writeGroupedData(const std::string& name, const std::string& groupName,
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

void jino::NetCDFWriter::writeUngroupedData(const std::string& name, NetCDFFile& file,
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

jino::NetCDFFile& jino::NetCDFWriter::getFile() {
  if (file_ == nullptr) {
    init();
  }
  return *file_;
}
