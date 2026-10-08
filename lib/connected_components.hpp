#pragma once

#include "digraph.hpp"
#include "graph.hpp"
#include <cstdint>
#include <vector>

class CCUndirected {
  Graph graph{};
  std::vector<std::int64_t> cc{};
  int64_t count{};
  int64_t component_count{};

  int64_t id(const VertexID v) const;

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
  std::vector<int64_t> cc{};
  int64_t count{};
  int64_t component_count{};

  int64_t id(const VertexID v) const;

public:
  CCDirected();

  VertexID add_vertex();

  bool add_edge(const VertexID v, const VertexID w);

  bool is_connected(const VertexID v, const VertexID w) const;

  bool remove_vertex(const VertexID v);

  bool remove_edge(const VertexID v, const VertexID w);
};
