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



struct dual_tree_builder : public bfs_basic_visit_data {
	typedef iterator_property_map<std::vector<std::vector<TriFace*>>::iterator, EdgeIndexer> EdgeFaceMap;
	typedef iterator_property_map<TriFace*, VertexIndexer> DualVertexFaceMap;

	dual_tree_builder(Graph const &arg_g) : bfs_basic_visit_data(arg_g) {
		edge_faces_storage.resize(num_edges(g));
		edge_faces = make_iterator_property_map(edge_faces_storage.begin(), get(edge_index, g));

		// recall graph g is triangulated
		size_t total_faces = 2 * num_vertices(g) - 4;
		faces.reset(new TriFace[total_faces]);
	}

	void build_dual_tree() {
		int n = num_faces();
		dual_tree = Graph(n);
		VertexItr vi, vi_end;
		int i = 0;
		dual_v2f_map = make_iterator_property_map(faces.get(), get(vertex_index, dual_tree));
		for (boost::tie(vi, vi_end) = vertices(dual_tree); vi != vi_end; ++vi) {
			faces[i].dual_vertex = *vi;		// map a triangular face to a dual vertex
			dual_v2f_map[*vi] = faces[i];	// map a dual vertex to a triangular face
			i++;
		}
		for (int j = 0; j < num_faces(); j++) {
			std::cout << "dual vertices: " << faces[j].dual_vertex << std::endl;
		}
		EdgeItr eit, eit_end;
		TriFace f1, f2;
		for (boost::tie(eit, eit_end) = edges(g); eit != eit_end; ++eit) {
			if (!is_bfs_tree_edges[*eit]) {
				std::vector<TriFace*> incident_faces = edge_faces[*eit];
				f1 = *(incident_faces[0]);
				f2 = *(incident_faces[1]);
				add_edge(f1.dual_vertex, f2.dual_vertex, dual_tree);
			}

		}
		reset_edge_indices(dual_tree);
	}

	bool is_null(TriFace tf) {
		return (tf.vertices_on_face.get<0>() == boost::graph_traits<Graph>::null_vertex()) ? true : false;
	}

	void add_face(TriFace tf) {
		faces[faces_index] = tf;
		faces_index++;
	}

	int num_faces() {
		return faces_index;
	}

	EdgeFaceMap edge_faces; 
	DualVertexFaceMap dual_v2f_map;
	std::unique_ptr<TriFace[]> faces;
	Graph dual_tree;

private:
	int faces_index = 0;
	boost::tuple<Vertex, Vertex, Vertex> make_null_vertex_tuple(Vertex v) {
		boost::tuple<Vertex, Vertex, Vertex> null_vertices(v,v,v);
		return null_vertices;
	}
	std::vector<std::vector<TriFace*>> edge_faces_storage;
};



struct bfs_dual_tree_visitor :public default_bfs_visitor {
	dual_tree_builder &builder;

	bfs_dual_tree_visitor(dual_tree_builder &arg_builder) : builder(arg_builder) {}

	template < typename Edge, typename Graph >
	void tree_edge(Edge e, const Graph & g) const {
		builder.is_bfs_tree_edges[e] = true;
		std::cout << "Tree edge " << source(e, g) << "," << target(e, g) << std::endl;
	}

};
/*
struct dual_vetex_visitor : public planar_face_traversal_visitor {
	dual_tree_builder &builder;
	std::vector<Vertex> vertices_on_face;
	std::vector<Edge> edges_on_face;

	

};*/
struct face_visitor : public planar_face_traversal_visitor {
	
	dual_tree_builder &builder;
	std::vector<Vertex> vertices_on_face;
	std::vector<Edge> edges_on_face;

	face_visitor(dual_tree_builder &arg_builder) : builder(arg_builder) {}

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
			typedef boost::tuple<Vertex, Vertex, Vertex> VertexTuple;
			typedef boost::tuple<Edge, Edge, Edge> EdgeTuple;
			VertexTuple vertices_tp(vertices_on_face.at(0), vertices_on_face.at(1), vertices_on_face.at(2));
			EdgeTuple edges_tp(edges_on_face.at(0), edges_on_face.at(1), edges_on_face.at(2));

			TriFace tf(vertices_tp, edges_tp);
			builder.add_face(tf); //faces.push_back(tf);
			for (std::vector<Edge>::iterator eit = edges_on_face.begin(); eit != edges_on_face.end(); ++eit) {
				builder.edge_faces[*eit].push_back(&builder.faces[builder.num_faces() - 1]);
			}
		}

		// clear for the next iteration
		edges_on_face.clear();
		vertices_on_face.clear();
	}
};


// dfs the dual tree to find the separation edge
/*
struct sep_edge_locator :public default_dfs_visitor {
	std::vector<std::list<int>> cycle_holder;
	std::list<int>** cycles;
	sep_edge_locator(dual_tree_builder &arg_dt_builder) : dt_builder(arg_dt_builder) {
		int n = num_vertices(arg_dt_builder.dual_tree);
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
//		VertexIndexer vindex = get(vertex_index, g);
//		int u_index = vindex[u];
		if (degree(u, g) <= 1) { // u is a leaf face
			//tri_face incident_face = dt_builder.face_of_index(u_index);
			TriFace incident_face = dt_builder.dual_v2f_map[u];
/*			boost::tuple<int, int, int> incident_vertices = incident_face.get<0>();
			std::list<int> facial_cyce;
			facial_cyce.push_back(incident_vertices.get<0>());
			facial_cyce.push_back(incident_vertices.get<1>());
			facial_cyce.push_back(incident_vertices.get<2>());
			cycle_holder.push_back(facial_cyce);
			cycles[u_index] = &cycle_holder.back();
			std::cout << "The leaf cycle: ";
			for (std::list<int>::iterator fit = facial_cyce.begin(); fit != facial_cyce.end(); ++fit) {
				std::cout << *fit << "\t";
			}
			std::cout << std::endl; //
		}
		else {

		}
	}
private:
	int n;
	dual_tree_builder dt_builder;

};
*/

// precondition: g must be triangulated and source is the center of g
void find_low_radius_separator(Graph const&g, Vertex source) {
	dual_tree_builder dtb(g);
	face_visitor fvisitor(dtb);
	EmbeddingStorage em = find_planar_embedding(g);
	print_planar_embedding(g, em);
	planar_face_traversal(g, &em[0], fvisitor);
	
	bfs_dual_tree_visitor bfs_visitor(dtb);
	std::cout << "# faces: " << dtb.num_faces() << std::endl;
	dtb.levels[source] = 0; // set_bfs_level(source, 0);
	breadth_first_search(g, source, visitor(bfs_visitor));
	dtb.build_dual_tree();
	print_graph(dtb.dual_tree);

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
	/*		sep_edge_locator selocator(dtb);
	depth_first_search(dtb.dual_tree, visitor(selocator).color_map(get(vertex_color, dtb.dual_tree)).root_vertex(dfs_source));
	//, vertex_color_map(get(vertex_color, dtb.dual_tree))
*/
}


