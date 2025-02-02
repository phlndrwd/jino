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

#include "JsonReader.h"

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
  jino::JsonReader Reader;
  Garage garage = Reader.readState<Garage>();

  for (const auto& car : garage.cars_) {
    std::cout << "Make: " << car.make_ << ", Model: " << car.model_
                          << ", Temperatures: " << std::endl;
    for (std::size_t i = 0; i < car.engine_.pistons_.size(); ++i) {
      std::cout << "    Piston_" << std::to_string(i + 1) << ": "
                << car.engine_.pistons_.at(i).temperature_ << std::endl;
    }
  }

  return 0;
}
