cmake_minimum_required(VERSION 3.14)
project(main)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(runTests main.cc)
add_executable(runSpaceUtil SpaceUtil.cc)
add_executable(runQuadTree QuadTree.cc)

target_link_libraries(runTests PRIVATE gtest_main geometry_lib)
target_link_libraries(runSpaceUtil PRIVATE gtest_main geometry_lib)
target_link_libraries(runQuadTree PRIVATE gtest_main geometry_lib)

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using GCC
    target_link_libraries(runTests PRIVATE pthread tbb)
    target_link_libraries(runSpaceUtil PRIVATE pthread tbb)
    target_link_libraries(runQuadTree PRIVATE pthread tbb)
endif()

