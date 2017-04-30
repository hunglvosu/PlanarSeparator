#pragma once

#include "graph_def.h"

struct bfs_basic_visit_data {
	bfs_basic_visit_data(Graph const & arg_g);
	int get_bfs_level(Vertex v);
	void set_bfs_level(Vertex v, int v_level);

	Graph g;
	int num_vs;
	int num_es;
	bool *is_bfs_tree_edges;
	int max_level = -1;
	VertexIndexer vindexer;
	EdgeIndexer eindexer;
	int *levels;
};
EmbeddingStorage find_planar_embedding(Graph const &g);
void find_planar_separator(Graph const &g);

template <typename Graph, typename bfs_level_data>
void bfs_phase(Graph const &g, bfs_level_data &data);
