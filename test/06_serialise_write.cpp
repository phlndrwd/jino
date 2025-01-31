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

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

#include "Output.h"

using json = nlohmann::json;

class Piston {
 public:
  double temperature_;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Piston, temperature_)
};

class Engine {
 public:
  std::vector<Piston> pistons_;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Engine, pistons_)
};

class Car {
 public:
  std::string make_;
  std::string model_;
  Engine engine_;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Car, make_, model_, engine_)
};

class Garage {
 public:
  std::vector<Car> cars_;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Garage, cars_)
};

std::int32_t main() {
  std::cout << "Creating sample data..." << std::endl;
  Piston piston1 = {91.1};
  Piston piston2 = {92.2};
  Piston piston3 = {93.3};
  Piston piston4 = {94.4};
  Piston piston5 = {95.5};
  Piston piston6 = {96.6};
  Piston piston7 = {97.7};
  Piston piston8 = {98.8};
  Engine engine1 = {{piston1, piston2, piston4, piston4}};
  Engine engine2 = {{piston5, piston6, piston7, piston8}};

  Car car1 = {"Volkswagen", "Golf", engine1};
  Car car2 = {"Ford", "Focus", engine2};

  Garage garage = {{car1, car2}};

  std::cout << "Serialise to JSON and write to file..." << std::endl;
  jino::Output output;
  output.writeState(garage);
  std::cout << "Complete." << std::endl;

  return 0;
}
