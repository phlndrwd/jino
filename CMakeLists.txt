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

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
#set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")

file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/input)
file(COPY ${CMAKE_SOURCE_DIR}/input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

## Download required files
include(FetchContent)

set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/external/")
FetchContent_Declare(
  nlohmann
  GIT_REPOSITORY "https://github.com/nlohmann/json.git"
  GIT_TAG "v3.11.3"
)
FetchContent_MakeAvailable(nlohmann)

FetchContent_Declare(cpplint
  GIT_REPOSITORY "https://github.com/cpplint/cpplint.git"
  GIT_TAG "2.0.0"
)
FetchContent_MakeAvailable(cpplint)

include_directories(${CMAKE_SOURCE_DIR}/external/nlohmann-src/include/)
include_directories(${CMAKE_SOURCE_DIR}/include/)
include_directories(${CMAKE_SOURCE_DIR}/src)

## Setup and configure NetCDF-C++ library
set(INCLUDE_PATH "/usr/include")
if(IS_DIRECTORY ${INCLUDE_PATH})
  if(EXISTS ${INCLUDE_PATH}/netcdf)
    set(NetCDF_CXX_INCLUDE_DIR ${INCLUDE_PATH})
  else()
    message(FATAL_ERROR "NetCDFFile not found: ${INCLUDE_PATH}/netcdf")
  endif()
else()
  message(FATAL_ERROR "Path not found: ${INCLUDE_PATH}")
endif()

set(NetCDF_CXX_LIB_FILE "/usr/lib/x86_64-linux-gnu/libnetcdf_c++4.so")
if(EXISTS ${NetCDF_CXX_LIB_FILE})
  set(NetCDF_CXX_LIBRARIES ${NetCDF_CXX_LIB_FILE})
else()
  message(FATAL_ERROR "NetCDFFile not found: ${NetCDF_CXX_LIB_FILE}")
endif()

## Add local source and header files
list(APPEND JINO_SOURCES
  src/Buffer.cpp
  src/BufferBase.cpp
  src/Buffers.cpp
  src/Data.cpp
  src/Datum.cpp
  src/DatumBase.cpp
  src/JsonReader.cpp
  src/NetCDFData.cpp
  src/NetCDFFile.cpp
  src/NetCDFThreadWriter.cpp
  src/NetCDFWriter.cpp
  src/NetCDFWriterBase.cpp
  src/Output.cpp
  src/OutputThread.cpp
  src/ThreadPool.cpp
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
  include/NetCDFThreadWriter.h
  include/NetCDFWriter.h
  include/NetCDFWriterBase.h
  include/Output.h
  include/OutputThread.h
  include/ThreadPool.h
  include/ThreadQueues.h
  include/Types.h
)

set(TEST_SOURCES
  test/01_basic_read.cpp
  test/02_buffer_instantiate.cpp
  test/03_netcdf_write.cpp
  test/04_progressive_write.cpp
  test/05_thread_write.cpp
  test/06_serialise_write.cpp
  test/07_deserialize_read.cpp
  test/08_full_serial.cpp
  test/09_full_parallel.cpp
)

## Create library
find_package(Threads REQUIRED)
add_library(jino STATIC ${JINO_SOURCES})
target_include_directories(jino PUBLIC ${NetCDF_INCLUDE_DIRS} ${NetCDF_CXX_INCLUDE_DIR})
target_link_libraries(jino PUBLIC Threads::Threads ${NetCDF_CXX_LIBRARIES})

## Create tests
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
add_compile_options(-Wall -Wextra -Wpedantic)
