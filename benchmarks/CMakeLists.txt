cmake_minimum_required(VERSION 3.14)
project(main)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

find_package(benchmark REQUIRED)

add_executable(runInsertBenchmark Insert.cc Utils.h)
add_executable(runQueryBenchmark Query.cc Utils.h)

target_link_libraries(runInsertBenchmark PRIVATE benchmark::benchmark gtest_main geometry_lib)
target_link_libraries(runQueryBenchmark PRIVATE benchmark::benchmark gtest_main geometry_lib)


if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using GCC
    target_link_libraries(runInsertBenchmark PRIVATE pthread tbb)
    target_link_libraries(runQueryBenchmark PRIVATE pthread tbb)
endif()

