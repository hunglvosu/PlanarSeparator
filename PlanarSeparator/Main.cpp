#include <iostream>

#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/is_straight_line_drawing.hpp>
#include <boost/graph/chrobak_payne_drawing.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/make_maximal_planar.hpp>
#include <boost/graph/make_biconnected_planar.hpp>
#include <boost/graph/breadth_first_search.hpp>


#include "graph_def.h"
#include "utils.h"
#include "separator.h"
#include "test_generator.h"
#include "triangulation.h"

using namespace std;

Graph load_hard_coded_graph() {
	Graph g(8);
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
	return g;
}




int main() {
	//Graph g = load_hard_coded_graph();
	Graph g = generate_triangulated_grid(3);
	//print_graph(g);
	//cout << "#vertices: " << num_vertices(g) << "\t #edges:" << num_edges(g) << " should have:" << 3 * num_vertices(g) - 6 << " edges" << endl;
	find_planar_separator(g);
	getchar();
	return 0;
}

