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

	std::array<std::list<int>,5> data_holder;
	std::list<int>** ptr_holder = new std::list<int>*[5];
	
};

struct list_modifier {
	int i = 0;
	list_holder &holder;
	list_modifier(list_holder &arg_holder) : holder(arg_holder) { }
	void make_a_list() {
		std::list<int> list1;
		list1.push_back(1);
		list1.push_back(2);
		list1.push_back(3);
		//if (holder.data_holder.size() < 3) holder.data_holder.resize(3);
		//holder.data_holder.push_back(list1);
		holder.data_holder[i] = list1;
		//ptr_holder[i] = &list1;
		holder.ptr_holder[i] = &holder.data_holder[i];
		cout << "i :" << i << endl;
		i++;
	}
	void change(std::list<int> *list1) {
		(*list1).push_back(4);
		(*list1).push_back(5);
		(*list1).push_back(6);
		(*list1).push_front(-1);
		(*list1).push_front(-2);
		(*list1).push_front(-3);


	}
};*/

int main() {
	//Graph g = load_hard_coded_graph();
	Graph g = generate_triangulated_grid(3);
	//print_graph(g);
	cout << "#vertices: " << num_vertices(g) << "\t #edges:" << num_edges(g) << " should have:" << 3 * num_vertices(g) - 6 << " edges" << endl;
	//find_planar_separator(g);
	Vertex s = *(vertices(g).first);
	Vertex t = *(vertices(g).second);
	//print_graph(g);
	//find_low_radius_separator(g, s);
	//find_planar_separator(g);


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
	list_modifier modif(holder);
	modif.make_a_list();
	if (holder.ptr_holder[0] == &holder.data_holder.at(0)) {
		cout << "the pointer is preserved" << endl;
	}
	else {
		cout << "the pointer changed" << endl;
	}
	modif.change(holder.ptr_holder[0]);
	if (holder.ptr_holder[0] == &holder.data_holder.at(0)) {
		cout << "the pointer is preserved" << endl;
	}
	else {
		cout << "the pointer changed" << endl;
	}
	modif.make_a_list();
	cout << "holder size: " << holder.data_holder.size() << endl;
	//std::list<int> list1 = *(holder.ptr_holder[0]);
	//std::list<int> list1 = holder.data_holder.at(0);
	if (holder.ptr_holder[0] == &holder.data_holder[0]) {
		cout << "the pointer is preserved" << endl;
	}
	else {
		cout << "the pointer changed" << endl;
	}
	std::list<int>* list1 = holder.ptr_holder[0];
	for (std::list<int>::iterator lit = (*list1).begin(); lit != (*list1).end(); ++lit) {
		std::cout << *lit << "\t";
	}
/*	std::cout << std::endl;
	holder.data_holder.at(1).splice(holder.data_holder.at(1).end(), holder.data_holder.at(0));
	list1 = holder.data_holder.at(1);
	for (std::list<int>::iterator lit = list1.begin(); lit != list1.end(); ++lit) {
		std::cout << *lit << "\t";
	}

	std::unique_ptr<int[]> my_array;
	my_array.reset(new int[5]);
	my_array[0] = 0;
	my_array[1] = 0;
	my_array[2] = 0;
	my_array[3] = 0;
	my_array[4] = 0;
	
	std::list<int> mylist;
	mylist.push_back(1);
	mylist.push_back(2);
	mylist.push_back(3);
	mylist.push_back(4);
	mylist.push_back(5);
	int x = mylist.front();
	int y = *(std::next(mylist.begin()));
	int z = *(std::next(mylist.rbegin()));
	cout << "x :" << x << "\t y:" << y <<  "\t z:" << z <<std::endl;
////////////////////////////////////////////////////////////////////////
	std::vector<int> testList;
	testList.push_back(1);
	testList.push_back(2);
	testList.push_back(3);
	std::vector<int>* listptr;
	listptr = &testList;
	std::cout << "Action 1: Address of the list: " << &testList;
	std::cout << "\t Address of the pointer: " << listptr << std::endl;

	testList.push_back(4);
	testList.push_back(5);
	testList.push_back(5);

	testList.push_back(5);
	testList.push_back(5);
	testList.push_back(5);
	testList.push_back(5);

	std::cout << "Action 2: Address of the list: " << &testList;
	std::cout << "\t Address of the pointer: " << listptr << std::endl;
	testList[10];

	testList.push_front(0);
	testList.push_front(-1);
	std::cout << "Action 3: Address of the list: " << &testList;
	std::cout << "\t Address of the pointer: " << listptr << std::endl;
	
	std::list<int> list1;
	list1.push_back(1);
	list1.push_back(2);
	list1.push_back(3);

	std::list<int> list2;
	list2.push_back(6);
	list2.push_back(7);
	list2.push_back(8); */

	getchar();
	return 0;
}

