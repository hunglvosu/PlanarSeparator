#pragma once
#include "graph_def.h"

void print_graph(Graph const &g);
void print_planar_embedding(Graph const &g, EmbeddingStorage &embedding);
void print_tri_face(tri_face f);

template <typename BFSVisitData>
void print_bfs_tree_edge(BFSVisitData &data);