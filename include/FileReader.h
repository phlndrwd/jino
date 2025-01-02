/******************************************************************************
* JSON In NetCDF Out                                                          *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef INCLUDE_FILEREADER_H_
#define INCLUDE_FILEREADER_H_

#include <string>

#include "nlohmann/json.hpp"

#include "Data.h"
#include "File.h"

using json = nlohmann::json;

namespace jino {
class FileReader {
 public:
  FileReader();

  void read(std::string&, std::string&);
  void getParams(jino::Data&);

 private:
  template <typename T>
  void setParam(jino::Data&, const std::string&, const T&);
  void setParam(jino::Data&, const std::string&, const std::uint8_t, const nlohmann::json&);

  File file_;
};
}  // namespace jino

#endif  // INCLUDE_FILEREADER_H_
