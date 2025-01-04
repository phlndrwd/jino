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

#include <string>

#include "Buffers.h"
#include "File.h"

void jino::Writer::toFile() {
  const std::string path = "/usr/share/test/test.nc";
  File file(path, netCDF::NcFile::replace);

  Buffers::get().forEachBuffer([](const std::string& name, BufferBase* const buffer) {
    std::cout << "Buffer name: " << name << std::endl;
    if (buffer != nullptr) {
      buffer->print();
    } else {
      throw std::runtime_error("Type mismatch or invalid cast.");
    }
  });
}
