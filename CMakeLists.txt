###############################################################################################
# Jino (JSON In NetCDF Out).                                                                  #
#                                                                                             #
# (C) Copyright 2025, Phil Underwood.                                                         #
#                                                                                             #
# Jino is free software: you can redistribute it and/or modify it under the terms of the GNU  #
# Lesser General Public License as published by the Free Software Foundation, either version  #
# 3 of the License, or (at your option) any later version.                                    #
#                                                                                             #
# Jino is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without   #
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the   #
# GNU Lesser General Public License for more details.                                         #
#                                                                                             #
# You should have received a copy of the GNU Lesser General Public License along with Jino.   #
# If not, see <https://www.gnu.org/licenses/>.                                                #
###############################################################################################

cmake_minimum_required(VERSION 3.5)

project(jino LANGUAGES CXX)

# Options
option(JINO_BUILD_TESTS "Build jino tests" OFF)

file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/input)
file(COPY ${CMAKE_SOURCE_DIR}/input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

## Download required files
include(FetchContent)

if(NOT TARGET nlohmann_json::nlohmann_json)
  FetchContent_Declare(
    nlohmann
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
  )
  FetchContent_MakeAvailable(nlohmann)
endif()

## Add local source and header files
list(APPEND JINO_SOURCES
  src/Buffer.cpp
  src/BufferBase.cpp
  src/Buffers.cpp
  src/Data.cpp
  src/Datum.cpp
  src/JsonReader.cpp
  src/NetCDFData.cpp
  src/NetCDFFile.cpp
  src/NetCDFWriter.cpp
  src/Output.cpp
  src/ThreadQueues.cpp
)

list(APPEND JINO_HEADERS
  include/Buffer.h
  include/BufferBase.h
  include/BufferKey.h
  include/Buffers.h
  include/Constants.h
  include/Data.h
  include/Datum.h
  include/DatumBase.h
  include/JsonReader.h
  include/NetCDFData.h
  include/NetCDFDim.h
  include/NetCDFFile.h
  include/NetCDFWriter.h
  include/Output.h
  include/ThreadQueues.h
  include/TypeMap.h
  include/Types.h
)

set(TEST_SOURCES
  test/01_basic_read.cpp
  test/02_buffer_instantiate.cpp
  test/03_netcdf_write.cpp
  test/04_thread_write.cpp
  test/05_deserialize_read.cpp
  test/06_serialize_write.cpp
  test/07_full_parallel.cpp
)

## Workaround for missing NetCDF::NetCDF_CXX target
find_path(NetCDF_CXX_INCLUDE_DIR
  NAMES netcdf
  PATHS /usr/include
)

find_library(NetCDF_CXX_LIBRARY
  NAMES netcdf_c++4
  PATHS /usr/lib /usr/lib/x86_64-linux-gnu
)

if(NOT NetCDF_CXX_INCLUDE_DIR OR NOT NetCDF_CXX_LIBRARY)
  message(FATAL_ERROR "NetCDF C++ library not found")
endif()

add_library(NetCDF::NetCDF_CXX UNKNOWN IMPORTED)

set_target_properties(NetCDF::NetCDF_CXX PROPERTIES
  IMPORTED_LOCATION "${NetCDF_CXX_LIBRARY}"
  INTERFACE_INCLUDE_DIRECTORIES "${NetCDF_CXX_INCLUDE_DIR}"
)

find_library(NetCDF_C_LIBRARY
  NAMES netcdf
  PATHS /usr/lib /usr/lib/x86_64-linux-gnu
)

if(NOT NetCDF_C_LIBRARY)
  message(FATAL_ERROR "NetCDF C library not found")
endif()

add_library(NetCDF::NetCDF_C UNKNOWN IMPORTED)

set_target_properties(NetCDF::NetCDF_C PROPERTIES
  IMPORTED_LOCATION "${NetCDF_C_LIBRARY}"
)

target_link_libraries(NetCDF::NetCDF_CXX
  INTERFACE
    NetCDF::NetCDF_C
)

## Create library
find_package(Threads REQUIRED)
add_library(jino STATIC ${JINO_SOURCES})
target_include_directories(jino PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(jino PUBLIC
  nlohmann_json::nlohmann_json
  Threads::Threads
  NetCDF::NetCDF_CXX
)
target_compile_features(jino PUBLIC cxx_std_23)
target_compile_options(jino PUBLIC -Wall -Wextra -Wpedantic)

## Create tests
if(JINO_BUILD_TESTS)
  option(JINO_ENABLE_CPPLINT "Enable cpplint checks" OFF)

  if(JINO_ENABLE_CPPLINT)
    FetchContent_Declare(cpplint
      GIT_REPOSITORY https://github.com/cpplint/cpplint.git
      GIT_TAG 2.0.2
    )
    FetchContent_MakeAvailable(cpplint)
  endif()

  enable_testing()
  add_test(NAME coding_norms
           COMMAND "${FETCHCONTENT_BASE_DIR}/cpplint-src/cpplint.py" --quiet --recursive
                   ${PROJECT_SOURCE_DIR}/src
                   ${PROJECT_SOURCE_DIR}/test)

  foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(EXECUTABLE_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${EXECUTABLE_NAME} ${TEST_SOURCE})
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE jino)
    add_test(NAME ${EXECUTABLE_NAME} COMMAND ${EXECUTABLE_NAME})
  endforeach()
endif()
