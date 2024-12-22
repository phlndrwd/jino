/******************************************************************************
* Buffered Output Thread (BOT)                                                *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include <iostream>

#include "FileReader.h"
#include "Parameters.h"

int main() {
  std::cout << "Reading parameters..." << std::endl;
  bot::Parameters params;
  bot::FileReader fileReader;
  fileReader.getParams(params);

  for (std::uint64_t i = 0; i < params.size(); i++) {
    std::cout << params[i].getValueStr() << std::endl;
  }

  std::cout << "Done." << std::endl;
  return 0;
}
