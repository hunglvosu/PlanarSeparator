#include <iostream>
#include "utils.h"
#include "graph_def.h"

using namespace std;

void print_planar_embedding(Graph const &g, EmbeddingStorage &embedding) {
	cout << "\n**********************  Embedding of a Planar Graph with Rotational System  **************************\n";
	VertexIndexer vindexer = get(vertex_index, g);
	for (vector<vector<Edge>>::size_type i = 0; i != embedding.size(); i++) {
		cout << i << ": ";
		for (std::vector<Edge>::iterator eit = embedding[i].begin(); eit != embedding[i].end(); ++eit) {
			Vertex u = source(*eit, g);
			Vertex v = target(*eit, g);
			if (vindexer[u] == i) cout << vindexer[v] << "\t";
			else cout << vindexer[u] << "\t";
		}
		std::cout << std::endl;
	}
	cout << "*********************************************************\n\n";
}

void print_graph(Graph const &g) {
	cout << "\n**********************  Graph  **************************\n";
	VertexItr vi, vi_end;
	VertexIndexer vindexer = get(vertex_index, g);
	for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
		cout << vindexer[*vi] << ": ";
		OutEdgeItr ei, ei_end;
		for (tie(ei, ei_end) = out_edges(*vi, g); ei != ei_end; ei++) {
			auto src = source(*ei, g);
			auto tar = target(*ei, g);
			if (src != *vi) cout << vindexer[src] << "\t";
			else cout << vindexer[tar] << "\t";
		}
		cout << endl;
	}
	cout << "*********************************************************\n\n";
}

void print_tri_face(tri_face f) {
	boost::tuple<int, int, int> v_on_f = f.get<0>();
	boost::tuple<int, int, int> e_on_f = f.get<1>();

	std::cout << "(" << v_on_f.get<0>() << "," << v_on_f.get<1>() << "," << v_on_f.get<2>() << ")";
}

template <typename BFSVisitData>
void print_bfs_tree_edge(BFSVisitData &data) {
	cout << "\n**********************  BFS tree edges  **************************\n";
	

}