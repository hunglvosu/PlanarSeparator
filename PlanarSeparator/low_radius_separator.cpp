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
#include "reversible_list.h"



struct dual_tree_builder : public bfs_basic_visit_data {
	typedef iterator_property_map<std::vector<std::vector<TriFace*>>::iterator, EdgeIndexer> EdgeFaceMap;
	typedef iterator_property_map<TriFace*, VertexIndexer> DualVertexFaceMap;

	dual_tree_builder(Graph const &arg_g) : bfs_basic_visit_data(arg_g) {
		edge_faces_storage.resize(num_edges(g));
		edge_faces = make_iterator_property_map(edge_faces_storage.begin(), get(edge_index, g));

		// recall graph g is triangulated
		size_t total_faces = 2 * num_vertices(g) - 4;
		//faces.reset(new TriFace[total_faces]);
		faces = new TriFace[total_faces];
	}

	void build_dual_tree() {
		int n = num_faces();
		dual_tree = Graph(n);
		VertexItr vi, vi_end;
		int i = 0;
		//dual_v2f_map = make_iterator_property_map(faces.get(), get(vertex_index, dual_tree));
		dual_v2f_map = make_iterator_property_map(faces, get(vertex_index, dual_tree));

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
//	std::unique_ptr<TriFace[]> faces;
	TriFace* faces;
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

struct sep_edge_locator :public default_dfs_visitor {
	//use array so that the pointer does not change
	// cannot use vector here
	srlist<Vertex>* cycle_holder;
	srlist<Vertex>** cycle_ptrs;
	dual_tree_builder &dt_builder;

	sep_edge_locator(dual_tree_builder &arg_dt_builder) : dt_builder(arg_dt_builder) {
		size_t n = num_vertices(arg_dt_builder.dual_tree);
		//cycles = new std::list<int>*[n];
		cycle_holder = new srlist<Vertex>[n];
		cycle_ptrs = new srlist<Vertex>*[n];
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
		std::cout << "Visiting : " << u << std::endl;
		if (degree(u, g) <= 1) { // u is a leaf face
			TriFace incident_face = dt_builder.dual_v2f_map[u];
//			std::cout << "The leaf cycle: ";
			print_tri_face(incident_face);
			std::cout << std::endl;
			std::array<Edge, 3> es;
			boost::tie(es[0], es[1], es[2]) = incident_face.edges_on_face;
			for (int i = 0; i < 3; i++) {
				if (!dt_builder.is_bfs_tree_edges[es[i]]) { // Case 1
					//std::cout << "non-tree edge: " << source(es[i], dt_builder.g) << " :" << target(es[i], dt_builder.g);
					//std::cout << std::endl;
					std::array<Vertex, 2> uv1;
					std::array<Vertex, 2> uv2;
					uv1[0] = source(es[(i + 1) % 3], dt_builder.g);
					uv1[1] = target(es[(i + 1) % 3], dt_builder.g);
					uv2[0] = source(es[(i + 2) % 3], dt_builder.g);
					uv2[1] = target(es[(i + 2) % 3], dt_builder.g);
					for (int j = 0; j < 2; j++) {
						for (int k = 0; k < 2; k++) {
							if (uv1[j] == uv2[k]) {
								cycle_holder[u].push_back(uv1[(j + 1) & 1]); // push (j+1)%2
								cycle_holder[u].push_back(uv1[j]);
								cycle_holder[u].push_back(uv2[(k + 1) & 1]); // push (k+1)%2
								break;
							}
						}
					}
				}
			}
			cycle_ptrs[u] = &cycle_holder[u];
//			cycle_holder[u].print();
		}
		else {
			typename graph_traits<Graph>::adjacency_iterator ai, ai_end;
//			std::cout << "Process " << u << std::endl;
			auto cmap = get(vertex_color, g);
			int n_children = 0;
			std::array<Vertex, 2> visited_children;
			// we gurantee the number of children of a vertex is at most 2 because the root of the dfs tree is a leaf
			// if we choose a degree-3 vertex to be the root, this condition will be broken at the root 
			for (boost::tie(ai, ai_end) = adjacent_vertices(u, g); ai != ai_end; ++ai) {
				if (cmap[*ai] == Color::black()) {
//					std::cout << *ai << " is visited" << std::endl;
					visited_children[n_children] = *ai;
					n_children++;
				}
			}
			if (n_children == 1) {
//				std::cout << "We are entering Case 2 and Case 3" << std::endl;
				std::array<Edge, 3> es;
				std::array<Vertex, 3> vs;
				TriFace dual_face = dt_builder.dual_v2f_map[u];
				print_tri_face(dual_face);
				std::cout << std::endl;
				boost::tie(es[0], es[1], es[2]) = dual_face.edges_on_face;
				boost::tie(vs[0], vs[1], vs[2]) = dual_face.vertices_on_face;
				// the first and the last vertex in the children path
				Vertex c_front = (*cycle_ptrs[visited_children[0]]).front();
				Vertex c_back = (*cycle_ptrs[visited_children[0]]).back();
				Vertex next_of_c_front = (*cycle_ptrs[visited_children[0]]).next(cycle_ptrs[visited_children[0]]->_head, 
					cycle_ptrs[visited_children[0]]->_head->_neighbors[0]);
				Vertex rev_next_of_c_back = (*cycle_ptrs[visited_children[0]]).next(cycle_ptrs[visited_children[0]]->_tail,
					cycle_ptrs[visited_children[0]]->_tail->_neighbors[0]);
//				std::cout << "c_front: " << c_front << "\t c_back: " << c_back
//					<< "\t next_of_c_front:" << next_of_c_front << "\t rev_next_of_c_back " << rev_next_of_c_back << std::endl;

				// match the endpoints of an edge of the dual_face
				// with two endpoints of the child path
				for (int i = 0; i < 3; i++) {
					if (vs[i] != c_front && vs[i] != c_back) {
						// found the top vertex vs[i]
						// check wether vs[i] is in the child path
						// if not we are in Case 2
						// otherwise, we are in Case 3
						if (vs[i] != next_of_c_front && vs[i] != rev_next_of_c_back) {
//							std::cout << "We are entering Case 2" << std::endl;
							for (int j = 0; j < 3; j++) { 
								// find the edge in the bfs_tree of the dual_face
								// note there is only one such edge
								if (dt_builder.is_bfs_tree_edges[es[j]]) {
									// check wheter whe should insert vs[i] to the front or the back of the cycle
									if (source(es[j], dt_builder.g) == c_front 
										|| target(es[j], dt_builder.g) == c_front) {
										(*cycle_ptrs[visited_children[0]]).push_front(vs[i]);
									}
									else {
										(*cycle_ptrs[visited_children[0]]).push_back(vs[i]);
									}
									cycle_ptrs[u] = cycle_ptrs[visited_children[0]];
//									(*cycle_ptrs[u]).print();
								}
							}
						}
						else {
//							std::cout << "We are entering Case 3" << std::endl;
							if (vs[i] == next_of_c_front) {
								(*cycle_ptrs[visited_children[0]]).remove_front();
							}
							else if (vs[i] == rev_next_of_c_back) {
								(*cycle_ptrs[visited_children[0]]).remove_back();
							}
							else {
								std::cout << "Something must be wrong here in Case 3!" << std::endl;
							}
							cycle_ptrs[u] = cycle_ptrs[visited_children[0]];
						}
					}
					//break;
				}
			}
			else {
//				std::cout << "We are entering Case 4" << std::endl;
				Vertex v = visited_children[0];
				Vertex w = visited_children[1];
				srlist<Vertex>* v_cycle_ptr = cycle_ptrs[v];
				srlist<Vertex>* w_cycle_ptr = cycle_ptrs[w];
//				std::cout << "v_cycle: ";
//				v_cycle.print();
//				std::cout << "w_cycle:";
//				w_cycle.print();
				// gurantee that v_cycle.back() == w_cycle.front()
				if ((*v_cycle_ptr).front() == (*w_cycle_ptr).front()) {
					(*v_cycle_ptr).reverse();
				}
				else if ((*v_cycle_ptr).front() == (*w_cycle_ptr).back()) {
					(*v_cycle_ptr).reverse();
					(*w_cycle_ptr).reverse();
				}
				else if ((*v_cycle_ptr).back() == (*w_cycle_ptr).back()) {
					(*w_cycle_ptr).reverse();
				}
				else  { //// v_cycle.back() == w_cycle.front()
					//do nothing
				}
				while ((*v_cycle_ptr).next(v_cycle_ptr->_tail, v_cycle_ptr->_tail->_neighbors[0]) == 
					(*w_cycle_ptr).next(w_cycle_ptr->_head, w_cycle_ptr->_head->_neighbors[0])) {
					(*v_cycle_ptr).remove_back();
					(*w_cycle_ptr).remove_front();
//					std::cout << "Next of v_cycle: " << v_cycle.back() << "\t Next of w_cycle: " << w_cycle.front() << std::endl;
				}
				(*w_cycle_ptr).remove_front();
				(*v_cycle_ptr).splice(*(w_cycle_ptr));
				cycle_ptrs[u] = cycle_ptrs[v];
				std::cout<< std::endl;

			}

		}
		(*cycle_ptrs[u]).print();
		std::cout << "cycle separator size:" << (*cycle_ptrs[u]).size() << std::endl;
	}
private:
	int n;
};


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

	// selecting a leaf vertex to be the root of dfs
	Vertex dfs_source;
	VertexItr vit, vit_end;
	for (boost::tie(vit, vit_end) = vertices(dtb.dual_tree); vit != vit_end; ++vit) {
		if (degree(*vit, dtb.dual_tree) == 1) {
			dfs_source = *vit;
			break;
		}
	}
	std::cout << "dfs_root: " << dfs_source << std::endl;
	sep_edge_locator selocator(dtb);
	depth_first_search(dtb.dual_tree, visitor(selocator).color_map(get(vertex_color, dtb.dual_tree)).root_vertex(dfs_source));
}

