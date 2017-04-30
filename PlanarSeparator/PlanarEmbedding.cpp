
#include <iostream>

#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/is_straight_line_drawing.hpp>
#include <boost/graph/chrobak_payne_drawing.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include "graph_def.h"

int embedding_example() {
	Graph g(7);
	add_edge(0, 1, g);
	add_edge(1, 2, g);
	add_edge(2, 3, g);
	add_edge(3, 0, g);
	add_edge(0, 4, g);
	add_edge(1, 5, g);
	add_edge(2, 6, g);
	add_edge(3, 7, g);
	add_edge(4, 5, g);
	add_edge(5, 6, g);
	add_edge(6, 7, g);
	add_edge(7, 4, g);

	EdgeIndexer e_index = get(edge_index, g);
	VertexIndexer v_index = get(vertex_index, g);

	graph_traits<Graph>::edges_size_type edge_count = 0;
	graph_traits<Graph>::edge_iterator ei, ei_end;
	for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
		put(e_index, *ei, edge_count++);

	//Test for planarity; compute the planar embedding as a side-effect
	typedef std::vector< Edge > vec_t;
	std::vector<vec_t> embedding(num_vertices(g));
	boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g,
		boyer_myrvold_params::embedding =
		&embedding[0]
	);
	for (std::vector<vec_t>::size_type i = 0; i != embedding.size(); i++) {
		std::cout << "Rotation around " << i << " is: ";
		for (std::vector<Edge>::iterator eit = embedding[i].begin(); eit != embedding[i].end(); ++eit) {
			Vertex u = source(*eit, g);
			Vertex v = target(*eit, g);
			std::cout << "( " << v_index[u] << "," << v_index[v] << "); ";
		}
		std::cout << std::endl;
	}

	getchar();
	return 0;
}