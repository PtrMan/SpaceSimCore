cmake_minimum_required (VERSION 2.8.11)

SET(EIGEN_INCLUDE "" CACHE PATH "Path to eigen include")

project (SpaceSimCore)

include_directories(${EIGEN_INCLUDE})
include_directories(${SpaceSimCore_SOURCE_DIR}/include)

add_library(SpaceSimCore src/physics/PhysicsEngine.cpp src/physics/PhysicsBody.cpp)

add_custom_target(
    copyBindings
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/SetupBindingsForStl.sh
)
add_dependencies(SpaceSimCore copyBindings)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow")
