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
  jino::Data params;
  jino::JsonReader Reader;
  Reader.readParams(params);
  std::cout << "Creating sample data..." << std::endl;
  Garage garage;
  const std::uint64_t maxPistons = 1000000;
  for (std::uint64_t i = 0; i < maxPistons; ++i) {
    Engine engine;
    for (std::uint64_t j = 0; j < 4; ++j) {
      engine.pistons_.emplace_back(static_cast<double>(j + 1));
    }
    const std::string name = std::to_string(i);
    garage.cars_.emplace_back("Make" + name, "Model" + name, engine);
  }
  std::cout << "Serialise to JSON and write to file..." << std::endl;
  jino::Output output;
  std::uint8_t writeState = params.getValue<std::uint8_t>(jino::consts::kWriteState);
  if (writeState == true) {
    output.writeState(garage);
  }
  std::cout << "Complete." << std::endl;

  return 0;
}
