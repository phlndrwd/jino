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
  bot::Parameters parameters;
  bot::FileReader fileReader;
  fileReader.getParams(parameters);

  return 0;
}
