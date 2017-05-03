#pragma once
#include "graph_def.h"
#include "low_radius_separator.h"

void print_graph(Graph const &g);
void print_planar_embedding(Graph const &g, EmbeddingStorage &embedding);
void print_tri_face(TriFace f);
void reset_edge_indices(Graph &g);

template <typename BFSVisitData>
void print_bfs_tree_edge(BFSVisitData &data);