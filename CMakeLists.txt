cmake_minimum_required(VERSION 3.1.0)
project(deCiPPher VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)

include(CTest)
enable_testing()
add_subdirectory(tests)

file(GLOB sources src/*.cpp)

add_library(deCiPPher deCiPPher.cpp)
add_executable(debug deCiPPher.cpp ${sources})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
