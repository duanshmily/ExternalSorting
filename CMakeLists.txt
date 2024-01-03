cmake_minimum_required(VERSION 3.0.0)
project(EXTERNAL_SORTING VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_BUILD_TYPE "Release")
add_subdirectory(src ${PROJECT_SOURCE_DIR}/bin)
add_compile_options(-std=c++17 -O2 -O3)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
