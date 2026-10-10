

#include "digraph.hpp"
#include <algorithm>
#include <cstddef>
#include <queue>
#include <vector>

Digraph::Digraph() = default;

VertexID Digraph::add_vertex() {
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

bool Digraph::add_edge(const VertexID v, const VertexID w) {
  if (!this->is_valid_vertex(v) || !this->is_valid_vertex(w)) {
    return false;
  }

  // A vertex is connected to itself and We do not keep entries for that.
  if (v == w) {
    return true;
  }

  this->adj[v].insert(w);
  this->_edge_count += 1;
  return true;
}

bool Digraph::remove_vertex(const VertexID id) {
  if (!this->is_valid_vertex(id)) {
    return false;
  }

  // make id reusable.
  this->reusable.insert(id);

  // Remove all the incoming connections.
  for (const auto v : this->adj[id]) {
    this->adj[v].erase(id);

    // Edges in digraphs are not bidirectional. If you can travel
    // bidirectionally then that means there are two different edges.
    this->_edge_count -= 1;
  }

  // Edges in digraphs are not bidirectional. If you can travel
  // bidirectionally then that means there are two different edges.
  this->_edge_count -= static_cast<int64_t>(adj[id].size());

  // Remove all the outgoing connections.
  this->adj[id].clear();

  return true;
}

size_t Digraph::vertex_count() const {
  return this->adj.size() - this->reusable.size();
}

size_t Digraph::edge_count() const { return this->_edge_count; }

bool Digraph::is_edge(const VertexID v, const VertexID w) const {
  if (!this->is_valid_vertex(v) || !this->is_valid_vertex(w)) {
    return false;
  }

  // means v is connected to w.
  return this->adj[v].contains(w);
}

bool Digraph::is_reusable(const VertexID v) const {
  return this->reusable.contains(v);
}

bool Digraph::is_valid_vertex(const VertexID v) const {
  return v >= 0 && v < static_cast<VertexID>(adj.size()) &&
         !reusable.contains(v);
}

bool Digraph::remove_edge(const VertexID v, const VertexID w) {
  if (!this->is_valid_vertex(v) || !this->is_valid_vertex(w)) {
    return false;
  }

  this->adj[v].erase(w);
  this->_edge_count -= 1;
  return true;
}

std::vector<VertexID> Digraph::adjacent_vertices(const VertexID v) const {
  if (this->is_valid_vertex(v)) {
    return {this->adj[v].begin(), this->adj[v].end()};
  }
  return {};
}

std::vector<VertexID> Digraph::path_dfs(const VertexID v,
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

void Digraph::dfs(const VertexID v, const VertexID w, std::vector<bool> &marked,
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

size_t Digraph::total_vertices() const { return this->adj.size(); }

std::vector<VertexID> Digraph::path_bfs(const VertexID v,
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

bool Digraph::is_connected(const VertexID v, const VertexID w) const {
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

std::vector<VertexID> Digraph::all_connected_vertices(const VertexID v) const {
  if (!this->is_valid_vertex(v)) {
    return {};
  }

  std::vector<bool> marked(this->total_vertices(), false);
  std::vector<VertexID> connected_components{};
  this->dfs_crawler(v, marked, connected_components);

  return connected_components;
}

void Digraph::dfs_crawler(const VertexID v, std::vector<bool> &marked,
                          std::vector<VertexID> &connected_components) const {

  marked[v] = true;
  connected_components.push_back(v);

  for (const auto adj : this->adjacent_vertices(v)) {
    if (!marked[adj]) {
      this->dfs_crawler(adj, marked, connected_components);
    }
  }
}

void Digraph::dfs_reverse_post_ord(
    const VertexID v, std::vector<bool> &marked,
    std::stack<VertexID> &reverse_post_ord) const {

  marked[v] = true;

  for (const auto x : this->adjacent_vertices(v)) {
    if (!marked[x]) {
      this->dfs_reverse_post_ord(x, marked, reverse_post_ord);
    }
  }

  reverse_post_ord.push(v);
}

std::vector<VertexID> Digraph::topological_sort() const {
  std::stack<VertexID> reverse_post_ord{};
  std::vector<bool> marked(this->total_vertices(), false);
  for (VertexID i{}; i < this->total_vertices() && this->is_valid_vertex(i);
       ++i) {
    if (!marked[i]) {
      this->dfs_reverse_post_ord(i, marked, reverse_post_ord);
    }
  }
  std::vector<VertexID> copy_reverse_post_ord{};
  while (!reverse_post_ord.empty()) {
    copy_reverse_post_ord.push_back(reverse_post_ord.top());
    reverse_post_ord.pop();
  }

  return copy_reverse_post_ord;
}
