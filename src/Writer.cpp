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

#include "Constants.h"
#include "Datum.h"
#include "File.h"
#include "Output.h"

void jino::Writer::toFile(File& file, const jino::Data& attrs, const jino::Data& params) const {
  addDims(file);
  addAttrs(file, attrs);
  addAttrs(file, params);
  addData(file);
}

void jino::Writer::addDims(File& file) const {
  Output::get().forEachDimension([&](const std::string& name, const std::uint64_t size) {
    file.addDimension(name, size);
  });
}

void jino::Writer::addAttrs(File& file, const jino::Data& attrs) const {
  attrs.forEachDatum([&](const std::string& key, DatumBase* const datum) {
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

void jino::Writer::addData(File& file) const {
  Output::get().forEachBuffer([&](const std::string& name, BufferBase* const buffer) {
    std::cout << "Buffer name: " << name << std::endl;
    if (buffer != nullptr) {
      std::string dimName = Output::get().getDimensionName(buffer->size());
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
