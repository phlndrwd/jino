###############################################################################
# JSON In, NetCDF Out                                                         #
#                                                                             #
# (C) Copyright 2024, Phil Underwood. All rights reserved.                    #
#                                                                             #
# This software is licensed under the terms of the 3-Clause BSD License       #
# which can be obtained from https://opensource.org/license/bsd-3-clause/.    #
###############################################################################

enable_testing()

include(FetchContent)
FetchContent_Declare(cpplint
  GIT_REPOSITORY "git@github.com:cpplint/cpplint.git"
  GIT_TAG "2.0.0"
  SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/cpplint"
)
FetchContent_MakeAvailable(cpplint)

add_test(NAME coding_norms
         COMMAND "${CMAKE_CURRENT_LIST_DIR}/cpplint/cpplint.py"
                 --exclude=${PROJECT_SOURCE_DIR}/test/cpplint
                 --quiet --recursive
                 ${PROJECT_SOURCE_DIR}/src
                 ${PROJECT_SOURCE_DIR}/test)

find_package(Threads REQUIRED)

set(TEST_SOURCES
  ${CMAKE_SOURCE_DIR}/test/first.cpp
)

foreach(TEST_SOURCE ${TEST_SOURCES})
  get_filename_component(EXECUTABLE_NAME ${TEST_SOURCE} NAME_WE)
  add_executable(${EXECUTABLE_NAME} ${TEST_SOURCE} ${JINO_SOURCES})
  target_link_libraries(${EXECUTABLE_NAME} PRIVATE Threads::Threads)

  add_test(NAME ${EXECUTABLE_NAME} COMMAND ${EXECUTABLE_NAME})
endforeach()

add_compile_options(-Wall -Wextra -Wpedantic)
