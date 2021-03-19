cmake_minimum_required(VERSION 3.1)
project(main)


add_executable(main main.cc)
add_executable(SpaceUtil SpaceUtil.cc)

target_link_libraries(main PRIVATE Catch2 geometry_lib)
target_link_libraries(SpaceUtil PRIVATE Catch2 geometry_lib)

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using GCC
    target_link_libraries(main PRIVATE pthread tbb)
    target_link_libraries(SpaceUtil PRIVATE pthread tbb)
endif()

