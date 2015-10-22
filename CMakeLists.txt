cmake_minimum_required (VERSION 2.8.11)
project (SpaceSimCore)

include_directories(/home/r0b3/libs/eigen)
include_directories(${SpaceSimCore_SOURCE_DIR}/include)

add_library(SpaceSimCore src/physics/PhysicsEngine.cpp)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow")
