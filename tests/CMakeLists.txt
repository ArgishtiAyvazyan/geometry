cmake_minimum_required(VERSION 3.1)
project(main)


add_executable(main main.cc)
add_executable(RTreeUnitTests RTreeUnitTests.cc)

target_link_libraries(main PRIVATE Catch2 r_tree_lib)
target_link_libraries(RTreeUnitTests PRIVATE Catch2 r_tree_lib)

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using GCC
    target_link_libraries(main PRIVATE pthread tbb)
    target_link_libraries(RTreeUnitTests PRIVATE pthread tbb)
endif()

