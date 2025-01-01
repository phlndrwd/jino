###############################################################################
# JSON In NetCDF Out                                                          #
#                                                                             #
# (C) Copyright 2024, Phil Underwood. All rights reserved.                    #
#                                                                             #
# This software is licensed under the terms of the 3-Clause BSD License       #
# which can be obtained from https://opensource.org/license/bsd-3-clause/.    #
###############################################################################

cmake_minimum_required(VERSION 3.5)

project(jino LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

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
    message(FATAL_ERROR "File not found: ${INCLUDE_PATH}/netcdf")
  endif()
else()
  message(FATAL_ERROR "Path not found: ${INCLUDE_PATH}")
endif()

set(NetCDF_CXX_LIB_FILE "/usr/lib/x86_64-linux-gnu/libnetcdf_c++4.so")
if(EXISTS ${NetCDF_CXX_LIB_FILE})
  set(NetCDF_CXX_LIBRARIES ${NetCDF_CXX_LIB_FILE})
else()
  message(FATAL_ERROR "File not found: ${NetCDF_CXX_LIB_FILE}")
endif()

## Add local source and header files
list(APPEND JINO_SOURCES
  src/Buffer.cpp
  src/Data.cpp
  src/Datum.cpp
  src/File.cpp
  src/FileReader.cpp
)

list(APPEND JINO_HEADERS
  include/Buffer.h
  include/Constants.h
  include/Data.h
  include/Datum.h
  include/DatumBase.h
  include/File.h
  include/FileReader.h
)

set(TEST_SOURCES
  test/basic_read.cpp
  test/buffer_test.cpp
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
