/******************************************************************************
* Buffered Output Thread (BOT)                                                *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>

#include "Parameters.h"

namespace bot {

class FileReader {
public:
  FileReader();

  void read(std::string&, std::string&);
  void getParams(bot::Parameters&);

private:
};

}  // namespace bot

#endif // FILEREADER_H
