#include "connected_components.hpp"

CCUndirected::CCUndirected() = default;

VertexID CCUndirected::add_vertex() { return {}; }

bool CCUndirected::add_edge(const VertexID v, const VertexID w) { return {}; }

bool CCUndirected::is_connected(const VertexID v, const VertexID w) const {
  return {};
}

bool CCUndirected::remove_vertex(const VertexID v) { return {}; }

bool CCUndirected::remove_edge(const VertexID v, const VertexID w) {
  return {};
}

CCDirected::CCDirected() = default;
