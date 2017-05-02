#include <iostream>
#include <boost/graph/copy.hpp>
#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/is_straight_line_drawing.hpp>
#include <boost/graph/chrobak_payne_drawing.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/planar_face_traversal.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_dfs.hpp>
#include <boost/cstdlib.hpp>


#include <math.h>  
#include "graph_def.h"
#include "utils.h"
#include "test_utils.h"
#include "separator.h"
#include "low_radius_separator.h"


//typedef typename property_traits<VertexColorMap>::value_type ColorValue;
//typedef color_traits<ColorValue> Color;

struct dual_tree_builder : public bfs_basic_visit_data {
	typedef std::pair<tri_face, tri_face> tri_face_pair;

	dual_tree_builder(Graph const &arg_g):bfs_basic_visit_data(arg_g) {}

	int index_of_face(tri_face f) {
		return findexer[f];
	}

	tri_face face_of_index(int i) {
		return faces.at(i);
	}

	void build_dual_tree() {
		int n = (int)faces.size();
		dual_tree = Graph(n);
		for (int i = 0; i < faces.size(); i++) {
			findexer[faces.at(i)] = i;
		}
		std::map<Edge, tri_face_pair>::iterator ef_it;
		int eid;
		tri_face_pair face_pair;
		for (ef_it = edge_faces.begin(); ef_it != edge_faces.end(); ++ef_it) {
			eid = eindexer[ef_it->first];
			face_pair = ef_it->second;
			if (!is_bfs_tree_edges[eid]) {
				tri_face fu = face_pair.first;
				tri_face fv = face_pair.second;
				add_edge(findexer[fu], findexer[fv], dual_tree);
			}
		}
	}

	tri_face_pair make_defaul_face_pair() {
		boost::tuple<int, int, int> vertices1(-1, -1, -1);
		boost::tuple<int, int, int> edges1(-1, -1, -1);
		tri_face f1(vertices1, edges1);
		boost::tuple<int, int, int> vertices2(-1, -1, -1);
		boost::tuple<int, int, int> edges2(-1, -1, -1);
		tri_face f2(vertices2, edges2);
		tri_face_pair f(f1, f2);
		return f;
	}
	tri_face_pair make_defaul_face_pair(tri_face f1) {
		boost::tuple<int, int, int> vertices2(-1, -1, -1);
		boost::tuple<int, int, int> edges2(-1, -1, -1);
		tri_face f2(vertices2, edges2);
		tri_face_pair f(f1, f2);
		return f;
	}
	std::map<Edge, tri_face_pair> edge_faces;
	std::vector<tri_face> faces;
	std::map<tri_face, int> findexer;
	Graph dual_tree;

};

struct bfs_dual_tree_visitor :public default_bfs_visitor {
	dual_tree_builder &builder;

	bfs_dual_tree_visitor(dual_tree_builder &arg_builder) : builder(arg_builder) {}

	template < typename Edge, typename Graph >
	void tree_edge(Edge e, const Graph & g) const {
		int e_index = builder.eindexer[e];
		builder.is_bfs_tree_edges[e_index] = true;
		std::cout << "Tree edge " << source(e, g) << "," << target(e, g) << std::endl;
	}

};

struct face_visitor : public planar_face_traversal_visitor {
	
	face_visitor(dual_tree_builder &arg_builder) : builder(arg_builder) {}

	dual_tree_builder &builder;
	std::vector<Vertex> vertices_on_face;
	std::vector<Edge> edges_on_face;
	void next_vertex(Vertex v) {
		vertices_on_face.push_back(v);
	}
	void next_edge(Edge e) {
		edges_on_face.push_back(e);
	}
	void end_face() {
		if (vertices_on_face.size() != 3) {
			std::cout << "Input graph is not triangulated" << std::endl;
			exit(EXIT_FAILURE);
		}
		else {
			typedef std::pair<tri_face, tri_face> tri_face_pair;
			boost::tuple<int, int, int> vertices_tp(vertices_on_face.at(0), vertices_on_face.at(1), vertices_on_face.at(2));
			boost::tuple<int, int, int> edges_tp(builder.eindexer[edges_on_face.at(0)],
				builder.eindexer[edges_on_face.at(1)],
				builder.eindexer[edges_on_face.at(2)]);
			tri_face tf(vertices_tp, edges_tp);
			builder.faces.push_back(tf);
			for (std::vector<Edge>::iterator eit = edges_on_face.begin(); eit != edges_on_face.end(); ++eit) {
				std::map<Edge, tri_face_pair>::iterator face_it;
				face_it = builder.edge_faces.find(*eit);
				if ( face_it != builder.edge_faces.end()) {
					builder.edge_faces[*eit].second = tf;
				}
				else {
					builder.edge_faces[*eit] = builder.make_defaul_face_pair(tf);
				}
			}
		}

		// clear for the next iteration
		edges_on_face.clear();
		vertices_on_face.clear();
	}
};


// dfs the dual tree to find the separation edge

struct sep_edge_locator :public default_dfs_visitor {
	std::vector<std::list<int>> cycle_holder;
	std::list<int>** cycles;
	sep_edge_locator(dual_tree_builder & arg_dt_builder) : dt_builder(arg_dt_builder) {
		n = num_vertices(arg_dt_builder.dual_tree);
		cycles = new std::list<int>*[n];
	}

	void discover_vertex(Vertex u, const Graph &g) {
	
	}
	void tree_edge(Edge e, const Graph &g) {
		Vertex u = source(e, g);
		Vertex v = target(e, g);
		auto cmap = get(vertex_color, g);
		Vertex unexplore;
		if (cmap[u] == Color::white()) {
			unexplore = u;
		}
		else  unexplore = v;

		//std::cout << "Color of " << u << " is " << cmap[u] << std::endl;

	}

	void finish_vertex(Vertex u, const Graph &g) {
		VertexIndexer vindex = get(vertex_index, g);
		int u_index = vindex[u];
		if (degree(u, g) <= 1) { // u is a leaf face
			tri_face incident_face = dt_builder.face_of_index(u_index);
			boost::tuple<int, int, int> incident_vertices = incident_face.get<0>();
			std::list<int> facial_cyce;
			facial_cyce.push_back(incident_vertices.get<0>());
			facial_cyce.push_back(incident_vertices.get<1>());
			facial_cyce.push_back(incident_vertices.get<2>());
			cycle_holder.push_back(facial_cyce);
			cycles[u_index] = &facial_cyce;
			std::cout << "The leaf cycle: ";
			for (std::list<int>::iterator fit = facial_cyce.begin(); fit != facial_cyce.end(); ++fit) {
				std::cout << *fit << "\t";
			}
			std::cout << std::endl;
		}
		else {

		}
	}
private:
	int n;
	dual_tree_builder dt_builder;

};


// precondition: g must be triangulated and source is the center of g
void find_low_radius_separator(Graph const&g, Vertex source) {
	dual_tree_builder dtb(g);
	face_visitor fvisitor(dtb);
	EmbeddingStorage em = find_planar_embedding(g);
	print_planar_embedding(g, em);
	planar_face_traversal(g, &em[0], fvisitor);
	
	bfs_dual_tree_visitor bfs_visitor(dtb);
	dtb.set_bfs_level(source, 0);
	breadth_first_search(g, source, visitor(bfs_visitor));
	dtb.build_dual_tree();
	print_graph(dtb.dual_tree);
	// reset the indices of edges for dfs
	reset_edge_indices(dtb.dual_tree);
	
	// selecting a degree-3 vertex to be the root of dfs
	Vertex dfs_source;
	VertexItr vit, vit_end;
	for (boost::tie(vit, vit_end) = vertices(dtb.dual_tree); vit != vit_end; ++vit) {
		if (degree(*vit, dtb.dual_tree) == 3) {
			dfs_source = *vit;
			break;
		}
	}
	std::cout << "dfs_root: " << dfs_source << std::endl;
	sep_edge_locator selocator(dtb);
	depth_first_search(dtb.dual_tree, visitor(selocator).color_map(get(vertex_color, dtb.dual_tree)).root_vertex(dfs_source));
	//, vertex_color_map(get(vertex_color, dtb.dual_tree))
}


