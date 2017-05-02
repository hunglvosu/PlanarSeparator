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
#include "low_radius_separator.h"

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



/*
struct list_holder {

	std::vector<std::list<int>> data_holder;
	void make_a_list() {
		std::list<int> list1;
		list1.push_back(1);
		list1.push_back(2);
		list1.push_back(3);
		data_holder.push_back(list1);
	}
	void change(std::list<int> &list1) {
		list1.push_back(4);
		list1.push_back(5);
		list1.push_back(6);

	}
};
*/

int main() {
	//Graph g = load_hard_coded_graph();
	Graph g = generate_triangulated_grid(3);
	//print_graph(g);
	cout << "#vertices: " << num_vertices(g) << "\t #edges:" << num_edges(g) << " should have:" << 3 * num_vertices(g) - 6 << " edges" << endl;
	//find_planar_separator(g);
	Vertex s = *(vertices(g).first);
	Vertex t = *(vertices(g).second);
	//print_graph(g);
	find_low_radius_separator(g, s);



/*	std::list<int> list1;
	list1.push_back(1);
	list1.push_back(2);
	list1.push_back(3);
	for (std::list<int>::iterator lit = list1.begin(); lit != list1.end(); ++lit) {
		std::cout << *lit << "\t";
	}
	std::cout << std::endl;
	std::list<int> *list2;
	list2 = &list1;
	(*list2).push_back(4);
	(*list2).push_back(5);
	for (std::list<int>::iterator lit = list1.begin(); lit != list1.end(); ++lit) {
		std::cout << *lit << "\t";
	}

	std::list<int> list1;
	list1.push_back(1);
	list1.push_back(2);
	list1.push_back(3);
	//for (std::list<int>::iterator lit = list1.begin(); lit != list1.end(); ++lit) {
	//	std::cout << *lit << "\t";
	//}
	std::map<int, std::list<int>> mymap;
	mymap[0] = list1;
	mymap[1] = list1;
	mymap[1].push_back(4);
	mymap[1].push_back(5);
	for (std::list<int>::iterator lit = mymap[1].begin(); lit != mymap[1].end(); ++lit) {
		std::cout << *lit << "\t";
	}
	
	
	list_holder holder;
	holder.make_a_list();
	holder.change(holder.data_holder.at(0));
	holder.make_a_list();
	std::list<int> list1 = holder.data_holder.at(0);
	for (std::list<int>::iterator lit = list1.begin(); lit != list1.end(); ++lit) {
		std::cout << *lit << "\t";
	}
	std::cout << std::endl;
	holder.data_holder.at(1).splice(holder.data_holder.at(1).end(), holder.data_holder.at(0));
	list1 = holder.data_holder.at(1);
	for (std::list<int>::iterator lit = list1.begin(); lit != list1.end(); ++lit) {
		std::cout << *lit << "\t";
	}*/
	getchar();
	return 0;
}

