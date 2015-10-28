cmake_minimum_required (VERSION 2.8.11)

SET(EIGEN_INCLUDE "" CACHE PATH "Path to eigen include")

project (SpaceSimCore)

include_directories(${EIGEN_INCLUDE})
include_directories(${SpaceSimCore_SOURCE_DIR}/include)

add_library(SpaceSimCore src/physics/PhysicsEngine.cpp src/physics/PhysicsBody.cpp src/physics/InertiaHelper.cpp       src/collision/SphereRayHelper.cpp src/collision/RaySphereRelativeStructureOfArrays.cpp    src/math/RaySphere.cpp    src/memory/SmartRawMemory.cpp           src/celestialMathematics/StarGeneration.cpp src/celestialMathematics/Orbit.cpp src/celestialMathematics/HohmannTransfer.cpp)

add_custom_target(
    copyBindings
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/SetupBindingsForStl.sh
)
add_dependencies(SpaceSimCore copyBindings)

add_executable(testPerformanceFastParticles tests/performance/PerformanceFastParticles.cpp)
target_link_libraries(testPerformanceFastParticles SpaceSimCore)



# TODO -march=native or the processor -march=corei7
#set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow   -ffast-math -O3 -funroll-loops -ftree-vectorize  -fno-math-errno -fomit-frame-pointer -flto")
## TOTEST<does this land in the link flags?> set(CMAKE_EXE_LINKER_FLAGS "-O3 -flto")

set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -Wextra -Wshadow   -ffast-math -O2 -funroll-loops -ftree-vectorize")
#set(CMAKE_CXX_FLAGS "-std=c++11   -O0 -g")
