cmake_minimum_required(VERSION 3.14)
project(main)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(runTests main.cc)
add_executable(runSpaceUtil SpaceUtil.cc)
add_executable(runQuadTree QuadTree.cc)

target_link_libraries(runTests PRIVATE ${GTEST_LIBRARIES} gtest_main geometry_lib)
target_link_libraries(runSpaceUtil PRIVATE ${GTEST_LIBRARIES} gtest_main geometry_lib)
target_link_libraries(runQuadTree PRIVATE ${GTEST_LIBRARIES} gtest_main geometry_lib)

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using GCC
    target_link_libraries(runTests PRIVATE pthread tbb)
    target_link_libraries(runSpaceUtil PRIVATE pthread tbb)
    target_link_libraries(runQuadTree PRIVATE pthread tbb)
endif()

