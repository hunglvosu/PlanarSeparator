
#include <iostream>
#include "test_utils.h"
using namespace std;

void check_edge_indexer(Graph &g, EdgeIndexer eindexer) {
	EdgeItr ei, ei_end;
	for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
		std::cout << "Edge" << *ei << ":" << eindexer[*ei] << "\t";
	std::cout << std::endl;
}

void check_vertex_indexer(Graph &g, VertexIndexer vindexer) {
	VertexItr vi, vi_end;
	for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
		std::cout << "Vertex" << *vi << ":" << vindexer[*vi] << "\t";
	std::cout << std::endl;
}

// junk code
/*
struct list_holder {

std::array<srlist<int>, 5> data_holder;
srlist<int>** ptr_holder = new srlist<int>*[5];

};

struct list_modifier {
int i = 0;
list_holder &holder;
list_modifier(list_holder &arg_holder) : holder(arg_holder) { }
void make_a_list() {
srlist<int> list1;
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
void change(srlist<int> *list1) {
(*list1).push_back(4);
(*list1).push_back(5);
(*list1).push_back(6);
(*list1).push_front(-1);
(*list1).push_front(-2);
(*list1).push_front(-3);


}
};*/
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
//	_srl_node<int> samplenode(3);
//	std::cout << "Sample node: " << samplenode._data << std::endl;

//	srlist<int> rev_list;
//	rev_list.debug();

/*list_holder holder;
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
srlist<int>* list1 = holder.ptr_holder[0];
(*list1).print();
holder.data_holder[1].splice(holder.data_holder[0]);
list1 = holder.ptr_holder[1];
(*list1).print();*/
