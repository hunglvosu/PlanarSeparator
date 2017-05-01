
#include <iostream>
#include <boost/graph/copy.hpp>
#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/is_straight_line_drawing.hpp>
#include <boost/graph/chrobak_payne_drawing.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <math.h>  
#include "graph_def.h"
#include "separator.h"
#include "utils.h"
#include "test_utils.h"

using namespace boost;
int separation_threshold;
EdgeIndexer eindexer;
VertexIndexer vindexer;

bfs_basic_visit_data::bfs_basic_visit_data(Graph const & arg_g) : g(arg_g)
{
	vindexer = get(vertex_index, g);
	eindexer = get(edge_index, g);
	num_vs = num_vertices(g);
	num_es = num_edges(g);
	is_bfs_tree_edges = new bool[num_es];
	levels = new int[num_vs];
	for (int i = 0; i < num_es; i++) is_bfs_tree_edges[i] = false;
	for (int i = 0; i < num_vs; i++) levels[i] = -1;
}

int bfs_basic_visit_data::get_bfs_level(Vertex v) {
	return levels[vindexer[v]];
}
void bfs_basic_visit_data::set_bfs_level(Vertex v, int v_level) {
	levels[vindexer[v]] = v_level;
}

struct bfs_level_data : public bfs_basic_visit_data {
	
	bfs_level_data(Graph const & arg_g) : bfs_basic_visit_data(arg_g) {}

	void debug() {
		std::cout << "Level: ";
		for (int i = 0; i < num_vs; i++) {
			std::cout << "(" << i << ":" << levels[i] << ")\t";
		}
		std::cout << std::endl;
		std::cout << "Level Data:";
		for (std::vector<std::vector<Vertex>>::iterator vi = bfs_levels.begin(); vi != bfs_levels.end(); ++vi) {
			std::cout << (*vi).size() << "\t";
		}
		std::cout << std::endl;

	}

	std::vector<std::vector<Vertex>> bfs_levels;
};

struct bfs_level_visitor :public default_bfs_visitor {
	bfs_level_data &data;

	bfs_level_visitor(bfs_level_data &inputdata) : data(inputdata) {}

	template < typename Edge, typename Graph >
	void tree_edge(Edge e, const Graph & g) const {
		int e_index = data.eindexer[e];
		data.is_bfs_tree_edges[e_index] = true;
		std::cout << "Tree edge" << std::endl;
		auto u = source(e, g);
		auto v = target(e, g);
		int u_level = data.get_bfs_level(u);
		int v_level = data.get_bfs_level(v);

		int parent_level = -1, child_level = -1;
		Vertex child;
		if (v_level == -1) {
			child = v;
			child_level = u_level + 1;
		}
		else {
			child = v;
			child_level = v_level + 1;
		}

		data.set_bfs_level(child, child_level);
		if (child_level > data.max_level) {
			data.max_level = child_level;
			std::vector<Vertex> vertices;
			vertices.push_back(child);
			data.bfs_levels.push_back(vertices);
		}
		else {
			data.bfs_levels.at(child_level).push_back(child);
		}
		//		std::cout << "tree edge " << u<< ", " << v << '\n';
	}

};

EmbeddingStorage find_planar_embedding(Graph const &g) {
	EmbeddingStorage estore(num_vertices(g));
	boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g,
		boyer_myrvold_params::embedding = &estore[0]);
	return estore;
}


void bfs_phase(Graph const &g, bfs_level_data &data) {
	// bfs the graph
	bfs_level_visitor blv(data);
	Vertex s = *(vertices(g).first);
	data.set_bfs_level(s, 0);
	std::vector<Vertex> vertices;
	vertices.push_back(s);
	data.bfs_levels.push_back(vertices);
	breadth_first_search(g, s, visitor(blv));
	data.max_level = data.bfs_levels.size() - 1;

	//data.debug();
}

void find_planar_separator(Graph const& g) {
	Graph cg;
	copy_graph(g, cg);
	eindexer = get(edge_index, cg);
	vindexer = get(vertex_index, cg);
	//check_edge_indexer(cg, eindexer);
	check_vertex_indexer(cg, vindexer);
	separation_threshold = (int)sqrt(num_vertices(cg));
	//std::cout << "sqrt(n) = " << threshold << std::endl;
	//print_graph(cg);
	EmbeddingStorage embedding = find_planar_embedding(cg);
	//print_planar_embedding(cg, embedding);
	bfs_level_data bfs_data(cg);
	bfs_phase(g, bfs_data);
	bfs_data.debug();
	if (bfs_data.max_level <= separation_threshold) {
		// precondition cg is triangulated


	}
	else {
		//processing bfs data 
	}

}
