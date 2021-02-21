project (r_tree)


add_library(r_tree_lib STATIC "Point.h" "RTree.h" "RTree.cc")

target_include_directories(r_tree_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(r_tree_lib INTERFACE )
