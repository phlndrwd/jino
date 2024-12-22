/******************************************************************************
* Buffered Output Thread (BOT)                                                *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <map>
#include <memory>
#include <string>

#include "DatumBase.h"

namespace bot {

class Parameters {
 public:
  Parameters();

  void setValue(const std::string&, const std::int8_t);
  void setValue(const std::string&, const std::int16_t);
  void setValue(const std::string&, const std::int32_t);
  void setValue(const std::string&, const std::int64_t);
  void setValue(const std::string&, const float);
  void setValue(const std::string&, const double);
  void setValue(const std::string&, const std::string);

 private:
  std::map<std::string, std::unique_ptr<bot::DatumBase>> values_;
};

}

#endif // PARAMETERS_H
