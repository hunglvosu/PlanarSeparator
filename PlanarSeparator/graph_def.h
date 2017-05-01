#pragma once
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <vector>

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>

using namespace boost;

typedef adjacency_list<vecS,
	vecS,
	undirectedS,
	property<vertex_index_t, int>,
	property<edge_index_t, int> > Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor	Vertex;
typedef boost::graph_traits<Graph>::vertex_iterator		VertexItr;
typedef boost::property_map<Graph, boost::vertex_index_t>::type VertexIndexer;

typedef boost::graph_traits<Graph>::edge_descriptor		Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator	OutEdgeItr;
typedef boost::property_map<Graph, boost::edge_index_t>::type EdgeIndexer;
typedef boost::graph_traits<Graph>::edge_iterator		EdgeItr;

typedef std::vector<std::vector<Edge>>					EmbeddingStorage;

typedef boost::tuple<boost::tuple<int, int, int>, boost::tuple<int, int, int>> tri_face;
//typedef std::tuple<int, int, int>						TriFace;


