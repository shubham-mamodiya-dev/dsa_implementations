

#include "digraph.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <queue>
#include <stack>
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

  for (const auto adj : this->adj[v]) {
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

    for (const auto adj : this->adj[current]) {
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

  for (const auto adj : this->adj[v]) {
    if (!marked[adj]) {
      this->dfs_crawler(adj, marked, connected_components);
    }
  }
}

void Digraph::dfs_reverse_post_ord(const VertexID v, std::vector<bool> &marked,
                                   std::vector<VertexID> &post_ord) const {

  marked[v] = true;

  for (const auto x : this->adj[v]) {
    if (!marked[x]) {
      this->dfs_reverse_post_ord(x, marked, post_ord);
    }
  }

  post_ord.push_back(v);
}

void Digraph::dfs_find_cycles(const VertexID v, std::vector<bool> &marked,
                              std::vector<VertexID> &edge_to,
                              std::vector<bool> &on_stack,
                              std::stack<VertexID> &cycle) const {

  on_stack[v] = true;
  marked[v] = true;

  for (auto const w : this->adj[v]) {
    bool found_cycle{!cycle.empty()};
    if (found_cycle) {
      return;
    } else if (!marked[w]) {
      edge_to[w] = v;
      this->dfs_find_cycles(w, marked, edge_to, on_stack, cycle);
    } else if (on_stack[w]) {
      cycle = std::stack<VertexID>{};
      for (VertexID x = v; x != w; x = edge_to[w]) {
        cycle.push(x);
      }
      cycle.push(w);
      cycle.push(v);
    }
  }

  on_stack[v] = false;
}
void Digraph::dfs_kosaraju_scc(const VertexID v, const int64_t scc_id,
                               std::vector<bool> &marked,
                               std::vector<int64_t> scc) const {

  marked[v] = true;
  scc[v] = scc_id;

  for (const VertexID w : this->adj[v]) {
    if (!marked[w]) {
      this->dfs_kosaraju_scc(w, scc_id, marked, scc);
    }
  }
}

std::vector<VertexID> Digraph::topological_sort() const {
  if (!this->is_DAG()) {
    return {};
  }
  return this->reverse_post_ord();
}
bool Digraph::has_cycle() const { return get_cycle().empty(); }

bool Digraph::is_DAG() const { return !this->has_cycle(); }

Digraph Digraph::reverse_digraph() const {

  Digraph reversed{*this};

  for (VertexID v = 0; v < this->total_vertices(); ++v) {
    for (const VertexID w : this->adj[v]) {
      reversed.remove_edge(v, w);
      reversed.add_edge(w, v);
    }
  }

  return reversed;
}
std::vector<VertexID> Digraph::reverse_post_ord() const {
  std::vector<VertexID> post_order(this->post_ord());
  std::reverse(post_order.begin(), post_order.end());
  return post_order;
}

std::vector<VertexID> Digraph::post_ord() const {
  std::vector<VertexID> post_order{};
  std::vector<bool> marked(this->total_vertices(), false);
  for (VertexID i{}; i < this->total_vertices() && this->is_valid_vertex(i);
       ++i) {
    if (!marked[i]) {
      this->dfs_reverse_post_ord(i, marked, post_order);
    }
  }

  return post_order;
}

std::vector<int64_t> Digraph::kosaraju_shrir_scc() const {
  std::vector<bool> marked(this->total_vertices(), false);
  std::vector<int64_t> scc(this->total_vertices());
  int64_t scc_id{};

  for (const VertexID v : this->reverse_digraph().reverse_post_ord()) {
    if (!marked[v] && !this->is_reusable(v)) {
      this->dfs_kosaraju_scc(v, scc_id++, marked, scc);
    }
  }

  return scc;
}

std::vector<VertexID> Digraph::get_cycle() const {
  std::vector<bool> on_stack(this->total_vertices(), false);
  std::vector<bool> marked(this->total_vertices(), false);
  std::vector<VertexID> edge_to(this->total_vertices());
  std::stack<VertexID> cycle{};

  for (int i = 0; i < this->total_vertices(); ++i) {
    edge_to[i] = i;
  }

  for (VertexID i = 0; i < this->total_vertices(); ++i) {
    if (!marked[i] && !this->is_reusable(i)) {
      this->dfs_find_cycles(i, marked, edge_to, on_stack, cycle);
    }
  }

  std::vector<VertexID> copy_cycle{};
  while (!cycle.empty()) {
    copy_cycle.push_back(cycle.top());
    cycle.pop();
  }

  return copy_cycle;
}
