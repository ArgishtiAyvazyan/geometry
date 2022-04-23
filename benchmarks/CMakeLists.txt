cmake_minimum_required(VERSION 3.14)
project(main)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

find_package(benchmark REQUIRED)

add_executable(runBenchmark main.cc)

target_link_libraries(runBenchmark PRIVATE benchmark::benchmark gtest_main geometry_lib)


if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using GCC
    target_link_libraries(runBenchmark PRIVATE pthread tbb)
endif()

