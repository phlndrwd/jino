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

#ifndef OUTPUT_H
#define OUTPUT_H

#include <cstdint>
#include <memory>
#include <string>

#include "NetCDFWriterBase.h"

namespace jino {

class Output {
 public:
  Output(const std::uint8_t = true);

  const std::string& getDate() const;
  NetCDFWriterBase& getWriter() const;

 private:
  void init() const;

  const std::string date_;
  std::unique_ptr<NetCDFWriterBase> writer_;
};

}  // namespace jino

#endif // OUTPUT_H
