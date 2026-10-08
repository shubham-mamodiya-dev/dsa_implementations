#pragma once

#include "digraph.hpp"
#include "graph.hpp"
#include <cstddef>
#include <vector>

class CCUndirected {
  Graph graph{};
  std::vector<VertexID> cc{};
  size_t count{};
  size_t component_count{};

public:
  CCUndirected();

  VertexID add_vertex();

  bool add_edge(const VertexID v, const VertexID w);

  bool is_connected(const VertexID v, const VertexID w) const;

  bool remove_vertex(const VertexID v);

  bool remove_edge(const VertexID v, const VertexID w);
};

class CCDirected {
  Digraph digraph{};
  std::vector<VertexID> cc{};
  size_t count{};

public:
  CCDirected();

  VertexID add_vertex();

  bool add_edge(const VertexID v, const VertexID w);

  bool is_connected(const VertexID v, const VertexID w) const;

  bool remove_vertex(const VertexID v);

  bool remove_edge(const VertexID v, const VertexID w);
};
