/**
 * @file
 * @brief This file contains implementations of digraph and some algorithms
 * involving digraph.
 */

#pragma once

#include <cstdint>
#include <unordered_set>
#include <vector>

using VertexID = int64_t;
class Digraph {
protected:
  std::vector<std::unordered_set<VertexID>> adj;
  /**
   * @brief recently_removed keeps those vertices that are deleted explicitly
   * from the graph. These vertices are re-utilized afterwards.
   */
  std::unordered_set<VertexID> reusable;
  int64_t _edge_count = 0;

  void dfs(const VertexID v, const VertexID w, std::vector<bool> &marked,
           std::vector<VertexID> &edge_to) const;

  void dfs_crawler(const VertexID v, std::vector<bool> &marked,
                   std::vector<VertexID> &connected_components) const;

public:
  Digraph();

  /**
   * @brief it adds vertex then returns its id. This id will be used for
   * interacting with it.
   *
   * @return int64_t as VertexID.
   */
  VertexID add_vertex();

  /**
   * @brief It adds edge between vertex v and vertex w only if v and w exists
   * in the digraph.
   *
   * @return true if edge is added else false. false may mean two things either
   * v or w doesn't exist in the digraph or one of them was recently removed.
   */
  bool add_edge(const VertexID v, const VertexID w);

  bool remove_vertex(const VertexID id);

  /**
   * @brief Calculates the total number of Vertices. It doesn't count vertices
   * that were deleted before.
   */
  size_t vertex_count() const;

  /**
   * @brief Calculates the count of edges in the digraph. It does not count and
   * edge for self loop for example it does not count for x is connected to x.
   */
  size_t edge_count() const;

  /**
   * @brief Checks for v having an edge to w.
   */
  bool is_edge(const VertexID v, const VertexID w) const;

  /**
   * @brief It checks if the given vertex is reusable. The vertices that were
   * deleted from the digraph are re-used.
   */
  bool is_reusable(const VertexID v) const;

  bool remove_edge(const VertexID v, const VertexID w);

  std::vector<VertexID> adjacent_vertices(const VertexID v) const;

  size_t total_vertices() const;

  std::vector<VertexID> path_dfs(const VertexID v, const VertexID w) const;
  std::vector<VertexID> path_bfs(const VertexID v, const VertexID w) const;

  bool is_connected(const VertexID v, const VertexID w) const;

  std::vector<VertexID> all_connected_vertices(const VertexID v) const;
  bool is_valid_vertex(const VertexID v) const;
};
