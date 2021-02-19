cmake_minimum_required(VERSION 3.1)
project(main)


add_executable(main main.cc)

target_link_libraries(main PRIVATE Catch2)

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using GCC
    target_link_libraries(main PRIVATE pthread tbb)
endif()

target_compile_features(main PUBLIC cxx_std_17)
