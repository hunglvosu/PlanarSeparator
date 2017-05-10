#pragma once

#include "graph_def.h"


struct bfs_basic_visit_data {
	typedef iterator_property_map<std::vector<bool>::iterator, EdgeIndexer> EdgeMarkerMap;
	typedef iterator_property_map<std::vector<int>::iterator, VertexIndexer> LevelMap;


	bfs_basic_visit_data(Graph const & arg_g);
	int get_bfs_level(Vertex v);
	void set_bfs_level(Vertex v, int v_level);

	Graph g;
	int num_vs ;
	int num_es;
	EdgeMarkerMap is_bfs_tree_edges;
	//bool *is_bfs_tree_edges;
	int max_level = -1;
	LevelMap levels;
private:
	std::vector<bool> e_marker_storage;
	std::vector<int> level_storage;
//	int *levels;
};
EmbeddingStorage find_planar_embedding(Graph const &g);
void find_planar_embedding(Graph const &g, EmbeddingStorage &estorage);

void find_planar_separator(Graph const &g);

template <typename Graph, typename bfs_level_data>
void bfs_phase(Graph const &g, bfs_level_data &data);
