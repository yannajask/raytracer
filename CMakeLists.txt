cmake_minimum_required(VERSION 4.0.2)

project(Raytracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenMP_C_FLAGS "-fopenmp")
set(OpenMP_CXX_FLAGS "-fopenmp -Wall -g -O3")
set(OpenMP_C_LIB_NAMES "omp")
set(OpenMP_CXX_LIB_NAMES "omp")
set(OpenMP_omp_LIBRARY "/opt/homebrew/opt/llvm/lib/libomp.dylib")

find_package(OpenMP REQUIRED)

add_executable(raytracer src/main.cc)
target_link_libraries(raytracer PRIVATE OpenMP::OpenMP_CXX)
include_directories(src)