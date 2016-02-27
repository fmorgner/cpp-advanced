set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/week01")

include_directories(include)

add_executable(distance_in_meters src/dim.cpp)
add_executable(speed src/spd.cpp)

