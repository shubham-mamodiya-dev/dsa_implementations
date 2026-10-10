add_library("dsa" STATIC
  graph.cpp
  graph.hpp
  digraph.cpp
  digraph.hpp
  connected_components.cpp
  connected_components.hpp
)

target_include_directories("dsa" INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
