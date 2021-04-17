project (geometry)


add_library(geometry_lib STATIC "Point.h" "Rect.h" "Square.h" "main.cc" "SimplePolygon.h" "Polygon.h")

target_include_directories(geometry_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(geometry_lib INTERFACE )
