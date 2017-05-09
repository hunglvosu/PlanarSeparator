

#include <boost/graph/make_maximal_planar.hpp>
#include <boost/graph/make_biconnected_planar.hpp>
#include "test_generator.h"
#include "separator.h"
#include "triangulation.h"

using namespace std;
// generate a size*size grid
Graph generate_grid(int size) {
	int n = size*size;
	Graph g(n);
	int k = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			k = i*size + j;
			if (j != size - 1) {
				add_edge(k, k + 1, g);
			}
			if (i != size - 1) {
				add_edge(k, k + size, g);
			}
		}
	}
	return g;
}

Graph generate_triangulated_grid(int size) {
	Graph g = generate_grid(size);
	// indexing edges of the graph
	EdgeIndexer e_index = get(edge_index, g);
	graph_traits<Graph>::edges_size_type edge_count = 0;
	EdgeItr ei, ei_end;
	for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
		put(e_index, *ei, edge_count++);

	std::cout << "Planar embedding" << std::endl;
	EmbeddingStorage embedding = find_planar_embedding(g);
	std::cout << "Make maximal " << std::endl;
	make_maximal_planar(g, &embedding[0]);
	// update the edge indexer
	edge_count = 0;
	for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
		put(e_index, *ei, edge_count++);
	return g;
}