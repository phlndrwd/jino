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

#include "Writer.h"

#include <iostream>
#include <string>

#include "Buffers.h"
#include "Constants.h"
#include "File.h"

void jino::Writer::toFile(const jino::Data& attrs) {
  const std::string path = "/usr/share/test/test.nc";
  File file(path, netCDF::NcFile::replace);

  attrs.forEachDatum([&](const std::string& key, const DatumBase& datum) {
    file.addAttribute(key, datum.getValueStr());
  });

  Buffers::get().forEachDimension([&](const std::string& name, const std::uint64_t size) {
    file.addDimension(name, size);
  });
  Buffers::get().forEachBuffer([&](const std::string& name, BufferBase* const buffer) {
    std::cout << "Buffer name: " << name << std::endl;
    if (buffer != nullptr) {
      std::string dimName = Buffers::get().getDimensionName(buffer->size());
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
        case consts::eLongDouble: {
          auto typedBuffer = static_cast<Buffer<long double>*>(buffer);
          file.addData<long double>(name, typedBuffer->getData());
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
