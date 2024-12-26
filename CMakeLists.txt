###############################################################################
# JSON In, NetCDF Out                                                         #
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
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

list(APPEND JINO_SOURCES
  src/Constants.h
  src/Datum.cpp
  src/Datum.h
  src/DatumBase.h
  src/FileReader.cpp
  src/FileReader.h
  src/Parameters.cpp
  src/Parameters.h
)

set(TEST_SOURCES
  test/first.cpp
)

enable_testing()

add_test(NAME coding_norms
         COMMAND "${FETCHCONTENT_BASE_DIR}/cpplint-src/cpplint.py" --quiet --recursive
                 ${PROJECT_SOURCE_DIR}/src
                 ${PROJECT_SOURCE_DIR}/test)

find_package(Threads REQUIRED)

foreach(TEST_SOURCE ${TEST_SOURCES})
  get_filename_component(EXECUTABLE_NAME ${TEST_SOURCE} NAME_WE)
  add_executable(${EXECUTABLE_NAME} ${TEST_SOURCE} ${JINO_SOURCES})
  target_link_libraries(${EXECUTABLE_NAME} PRIVATE Threads::Threads)

  add_test(NAME ${EXECUTABLE_NAME} COMMAND ${EXECUTABLE_NAME})
endforeach()

add_compile_options(-Wall -Wextra -Wpedantic)
