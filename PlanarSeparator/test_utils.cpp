
#include <iostream>
#include "test_utils.h"
using namespace std;

void check_edge_indexer(Graph &g, EdgeIndexer eindexer) {
	EdgeItr ei, ei_end;
	for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
		std::cout << "Edge" << *ei << ":" << eindexer[*ei] << "\t";
	std::cout << std::endl;
}

void check_vertex_indexer(Graph &g, VertexIndexer vindexer) {
	VertexItr vi, vi_end;
	for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
		std::cout << "Vertex" << *vi << ":" << vindexer[*vi] << "\t";
	std::cout << std::endl;
}
