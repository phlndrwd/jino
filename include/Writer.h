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

#ifndef INCLUDE_WRITER_H_
#define INCLUDE_WRITER_H_

#include "Data.h"
#include "File.h"

namespace jino {
class Buffers;

class Writer {
 public:
  Writer() = default;

  void toFile(File&, const jino::Data& attrs, const jino::Data& params) const;

 private:
  void addAttrs(File&, const Data&) const;
  void addDims(File&) const;
  void addData(File&) const;
};
}  // namespace jino

#endif // INCLUDE_WRITER_H_
