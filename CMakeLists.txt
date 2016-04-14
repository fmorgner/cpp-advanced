cmake_minimum_required(VERSION 3.0)
project(CplA CXX)

enable_testing()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "The build type" FORCE)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

set(OUTPUT_PREFIX "${CMAKE_BINARY_DIR}/products/${CMAKE_BUILD_TYPE}" CACHE STRING "The build product prefix")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PREFIX}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PREFIX}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PREFIX}/lib")

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -Werror -Wno-self-move -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g3 -fno-omit-frame-pointer -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

option(CPLA_ENABLE_TESTS "Enable unit tests" ON)

if(CPLA_ENABLE_TESTS)
  include(CUTE)
endif(CPLA_ENABLE_TESTS)

include_directories(SYSTEM third_party/cute)
include_directories(SYSTEM third_party/cpa/include)

add_subdirectory(third_party/cpa)
add_subdirectory(exercises)
