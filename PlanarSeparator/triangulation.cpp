#include <boost/graph/make_maximal_planar.hpp>
#include <boost/graph/make_biconnected_planar.hpp>
#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/is_straight_line_drawing.hpp>
#include <boost/graph/chrobak_payne_drawing.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/breadth_first_search.hpp>

#include "triangulation.h"
/*
template <typename Graph, typename Vertex>
struct face_triangulator : public planar_face_traversal_visitor {

	typedef typename std::vector<Vertex> vertex_vector_t;
	typedef typename vertex_vector_t::iterator face_iterator;

	face_triangulator(Graph& arg_g) :
		g(arg_g) {}

	// precondition: no self-loops and parallel-edges
	void next_vertex(Vertex v)
	{
		if (!vertices_on_face.empty() && vertices_on_face.front() == v) {
			return;
		}
		vertices_on_face.push_back(v);
	}
	void end_face() {
		std::cout << "Face:\t";
		for (face_iterator fi = vertices_on_face.begin(); fi != vertices_on_face.end(); ++fi) {
			std::cout << *fi << "\t";
		}
		std::cout << std::endl;
		if (vertices_on_face.size() <= 3)
		{
			// At most three vertices on this face - don't need to triangulate
			vertices_on_face.clear();
			return;
		}
		face_iterator fi_begin = vertices_on_face.begin();
		face_iterator fi = std::next(std::next(fi_begin));
		for (; std::next(fi) != vertices_on_face.end(); ++fi) {
			add_edge(*fi_begin, *fi, g);
		}
		//reset for the next face
		vertices_on_face.clear();
	}

private:
	Graph& g;
	vertex_vector_t vertices_on_face;
};


void triangulate(Graph &g, EmbeddingStorage &em) {
	face_triangulator<Graph, Vertex> ft(g);
	planar_face_traversal(g, &em[0], ft);
}*/