#include "connected_components.hpp"
#include "digraph.hpp"
#include <cstdint>

CCUndirected::CCUndirected() = default;

VertexID CCUndirected::add_vertex() {

  auto id{this->graph.add_vertex()};
  this->cc.push_back(this->count);
  ++this->count;
  ++this->component_count;
  return id;
}

bool CCUndirected::add_edge(const VertexID v, const VertexID w) {
  if (!this->graph.is_valid_vertex(v) || !this->graph.is_valid_vertex(w)) {
    return false;
  }

  if (v == w) {
    return true;
  }
  if (this->cc[v] != this->cc[w]) {
    --this->component_count;
  }

  VertexID chosen_vertex{this->id(v) < this->id(w) ? v : w};
  VertexID not_chosen_vertex{v == chosen_vertex ? w : v};

  auto chosen_component_id{this->cc[chosen_vertex]};

  for (const auto x : this->graph.all_connected_vertices(not_chosen_vertex)) {
    this->cc[x] = chosen_component_id;
  }

  this->graph.add_edge(v, w);
  return true;
}

bool CCUndirected::is_connected(const VertexID v, const VertexID w) const {
  return {};
}

bool CCUndirected::remove_vertex(const VertexID v) { return {}; }

bool CCUndirected::remove_edge(const VertexID v, const VertexID w) {
  return {};
}

int64_t CCUndirected::id(const VertexID v) const { return this->cc[v]; }

CCDirected::CCDirected() = default;

VertexID CCDirected::add_vertex() { return {}; }

bool CCDirected::add_edge(const VertexID v, const VertexID w) { return {}; }

bool CCDirected::is_connected(const VertexID v, const VertexID w) const {
  return {};
}

bool CCDirected::remove_vertex(const VertexID v) { return {}; }

bool CCDirected::remove_edge(const VertexID v, const VertexID w) { return {}; }

int64_t CCDirected::id(const VertexID v) const { return this->cc[v]; }
