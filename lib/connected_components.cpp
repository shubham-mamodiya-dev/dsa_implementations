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

VertexID CCDirected::add_vertex() { return {}; }

bool CCDirected::add_edge(const VertexID v, const VertexID w) { return {}; }

bool CCDirected::is_connected(const VertexID v, const VertexID w) const {
  return {};
}

bool CCDirected::remove_vertex(const VertexID v) { return {}; }

bool CCDirected::remove_edge(const VertexID v, const VertexID w) { return {}; }
