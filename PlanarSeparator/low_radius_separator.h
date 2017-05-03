#pragma once
#include "graph_def.h"

struct TriFace {
	typedef boost::tuple<Vertex, Vertex, Vertex> VertexTuple;
	typedef boost::tuple<Edge, Edge, Edge> EdgeTuple;
	VertexTuple vertices_on_face;
	EdgeTuple edges_on_face;
	Vertex dual_vertex;
	TriFace(){ 
		VertexTuple null_tuple(boost::graph_traits<Graph>::null_vertex(), boost::graph_traits<Graph>::null_vertex(), boost::graph_traits<Graph>::null_vertex());
		vertices_on_face = null_tuple;
	}
	TriFace(VertexTuple arg_v_tuple) : vertices_on_face(arg_v_tuple) { }
	TriFace(VertexTuple arg_v_tuple, EdgeTuple arg_e_tuple) : vertices_on_face(arg_v_tuple), edges_on_face(arg_e_tuple) {}
};

void find_low_radius_separator(Graph const&g, Vertex source);

