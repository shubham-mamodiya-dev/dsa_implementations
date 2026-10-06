

#include "graph.hpp"
#include <algorithm>
#include <cstddef>
#include <queue>
#include <vector>

Graph::Graph() = default;

VertexID Graph::add_vertex() {
  // First reusing vertices.
  if (!this->reusable.empty()) {
    const VertexID id = *this->reusable.begin();
    this->reusable.erase(id);
    return id;
  }

  const VertexID id = adj.size();
  adj.emplace_back();
  return id;
}

bool Graph::add_edge(const VertexID v, const VertexID w) {
  if (!this->is_valid_vertex(v) || !this->is_valid_vertex(w)) {
    return false;
  }

  // A vertex is connected to itself and We do not keep entries for that.
  if (v == w) {
    return true;
  }

  this->adj[v].insert(w);
  this->adj[w].insert(v);
  this->_edge_count += 1;
  return true;
}

bool Graph::remove_vertex(const VertexID id) {
  if (!this->is_valid_vertex(id)) {
    return false;
  }

  // make id reusable.
  this->reusable.insert(id);

  // Remove all the incoming connections.
  for (const auto v : this->adj[id]) {
    this->adj[v].erase(id);
  }

  // Edges in graphs are bidirectional. Deducing edges from one side is
  // accurate.
  this->_edge_count -= static_cast<int64_t>(adj[id].size());

  // Remove all the outgoing connections.
  this->adj[id].clear();

  return true;
}

size_t Graph::vertex_count() const {
  return this->adj.size() - this->reusable.size();
}

size_t Graph::edge_count() const { return this->_edge_count; }

bool Graph::is_edge(const VertexID v, const VertexID w) const {
  if (!this->is_valid_vertex(v) || !this->is_valid_vertex(w)) {
    return false;
  }

  // Edges in graphs are bidirectional. But, We are checking only for one
  // direction
  return this->adj[v].contains(w);
}

bool Graph::is_reusable(const VertexID v) const {
  return this->reusable.contains(v);
}

bool Graph::is_valid_vertex(const VertexID v) const {
  return v >= 0 && v < static_cast<VertexID>(adj.size()) &&
         !reusable.contains(v);
}

bool Graph::remove_edge(const VertexID v, const VertexID w) {
  if (!this->is_valid_vertex(v) || !this->is_valid_vertex(w)) {
    return false;
  }

  this->adj[v].erase(w);
  this->adj[w].erase(v);
  this->_edge_count -= 1;
  return true;
}

std::vector<VertexID> Graph::adjacent_vertices(const VertexID v) const {
  if (this->is_valid_vertex(v)) {
    return {this->adj[v].begin(), this->adj[v].end()};
  }
  return {};
}

std::vector<VertexID> Graph::path_dfs(const VertexID v,
                                      const VertexID w) const {
  if (!this->is_valid_vertex(v) || !this->is_valid_vertex(w)) {
    return {};
  }
  if (v == w) {
    return {v};
  }
  std::vector<bool> marked(this->total_vertices(), false);
  std::vector<VertexID> edge_to(this->total_vertices());

  for (int i = 0; i < this->total_vertices(); ++i) {
    edge_to[i] = i;
  }

  this->dfs(v, w, marked, edge_to);

  // if there is no path.
  if (marked[v] != marked[w]) {
    return {};
  }

  std::vector<VertexID> path{};
  auto current{w};
  while (current != v) {
    path.push_back(current);
    current = edge_to[current];
  }
  path.push_back(current);

  std::reverse(path.begin(), path.end());

  return path;
}

void Graph::dfs(const VertexID v, const VertexID w, std::vector<bool> &marked,
                std::vector<VertexID> &edge_to) const {

  marked[v] = true;

  for (const auto adj : this->adjacent_vertices(v)) {
    if (!marked[adj]) {

      // if we found the destination then just return.
      if (adj == w) {
        marked[w] = true;
        edge_to[w] = v;
        return;
      }

      this->dfs(adj, w, marked, edge_to);
      edge_to[adj] = v;
    }
  }
}

size_t Graph::total_vertices() const { return this->adj.size(); }

std::vector<VertexID> Graph::path_bfs(const VertexID v,
                                      const VertexID w) const {
  if (!this->is_valid_vertex(v) || !this->is_valid_vertex(w)) {
    return {};
  }

  if (v == w) {
    return {v};
  }
  std::queue<VertexID> frontier;
  std::vector<VertexID> edge_to(this->total_vertices());

  for (int i = 0; i < this->total_vertices(); ++i) {
    edge_to[i] = i;
  }

  std::vector<bool> marked(this->total_vertices(), false);

  frontier.push(v);
  marked[v] = true;

  auto found = false;
  while (!frontier.empty() && !found) {
    auto current{frontier.front()};

    for (const auto adj : this->adjacent_vertices(current)) {
      if (!marked[adj]) {
        frontier.push(adj);
        marked[adj] = true;
        edge_to[adj] = current;

        // stop if we found w.
        if (adj == w) {
          found = true;
          break;
        }
      }
    }
  }

  // We have to trace the path from w to v. Then reverse it to get a path from v
  // to w.
  std::vector<VertexID> path{};
  auto current{w};
  while (current != v) {
    path.push_back(current);
    current = edge_to[current];
  }
  path.push_back(current);

  std::reverse(path.begin(), path.end());

  return path;
}

bool Graph::is_connected(const VertexID v, const VertexID w) const {
  if (!this->is_valid_vertex(v) || !this->is_valid_vertex(w)) {
    return false;
  }
  if (v == w) {
    return true;
  }
  std::vector<VertexID> path{this->path_dfs(v, w)};

  // path_dfs always returns path if it exists.
  if (path.empty()) {
    return false;
  }

  return true;
}

std::vector<VertexID> Graph::all_connected_vertices(const VertexID v) const {
  if (!this->is_valid_vertex(v)) {
    return {};
  }

  std::vector<bool> marked(this->total_vertices(), false);
  std::vector<VertexID> connected_components{};
  this->dfs_crawler(v, marked, connected_components);

  return connected_components;
}

void Graph::dfs_crawler(const VertexID v, std::vector<bool> &marked,
                        std::vector<VertexID> &connected_components) const {

  marked[v] = true;
  connected_components.push_back(v);

  for (const auto adj : this->adjacent_vertices(v)) {
    if (!marked[adj]) {
      this->dfs_crawler(adj, marked, connected_components);
    }
  }
}
