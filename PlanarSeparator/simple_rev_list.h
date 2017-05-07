#pragma once
#include <iostream>
template<typename T>
struct _srl_node {
	std::array<_srl_node<T>*, 2> _srl_children;
	T _data{};
	int color;
	_srl_node();
	_srl_node(T arg_data);
};

template<typename T>
class srlist {
	_srl_node<T>* _head;
	_srl_node<T>* _tail;
	int _size;
public:
	srlist();
	void push_back(T elem);
//	void remove_back();
//	void push_front(T elem);
//	void remove_font();
	bool is_empty();
	int size();
	void print();

};
