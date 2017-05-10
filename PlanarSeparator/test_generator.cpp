

#include <boost/graph/make_maximal_planar.hpp>
#include <boost/graph/make_biconnected_planar.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>

#include "test_generator.h"
#include "separator.h"
#include "triangulation.h"
#include <ctime>


using namespace std;
// generate a size*size grid
void generate_grid(int size, Graph &g) {
	int n = size*size;
	using namespace std;
	clock_t begin = clock();
//	Graph g(n);
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
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "Time to add edges to a " << size*size << " grid is: " << elapsed_secs << endl;
	//return g;
}

void generate_triangulated_grid(int size, Graph &g) {
	std::cout << "generating a grid" << std::endl;
	using namespace std;
	clock_t begin = clock();
	//Graph g = generate_grid(size);
	generate_grid(size, g);
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "Time to generate a " << size*size << " grid is: " << elapsed_secs << endl;
	// indexing edges of the graph
	EdgeIndexer e_index = get(edge_index, g);
	graph_traits<Graph>::edges_size_type edge_count = 0;
	EdgeItr ei, ei_end;
	for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
		put(e_index, *ei, edge_count++);
	begin = clock();
	std::cout << "Planar embedding" << std::endl;
	EmbeddingStorage embedding(num_vertices(g));
	//find_planar_embedding(g, embedding);
	boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g,
		boyer_myrvold_params::embedding = &embedding[0]);
	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "Time to embed a " << size*size << " grid is: " << elapsed_secs << endl;

	begin = clock();
	std::cout << "Make maximal " << std::endl;
	make_maximal_planar(g, &embedding[0]);
	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "Time to triangulate a " << size*size << " grid is: " << elapsed_secs << endl;
	// update the edge indexer
	edge_count = 0;
	for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
		put(e_index, *ei, edge_count++);
	//return g;
}