#include "reversible_list.h"
#include<iostream>
#include <array>

/**
* A simply reversible list that supports reverse operation and splice operation both in O(1) time
* The internal implementation is a doubly-linked list without distinguishing between next pointer and previous pointer
*/
template<typename T>
struct _srl_node {
	std::array<_srl_node*, 2> _srl_children;
	T _data{};
	int color;
	_srl_node(){
		_srl_children[0] = nullptr;
		_srl_children[1] = nullptr;
		color = -1;
	}
	_srl_node(T arg_data) : _data(arg_data) {
		_srl_node();
	}
};

template<typename T>
class srlist {
	//_head._srl_children[1] and _tail._srl_children[1] are always nullptr
	_srl_node* _head;
	_srl_node* _tail;
	int _size;
public: 
	srlist() {	// default constructor
		_head = new _srl_node();
		_tail = new _srl_node();
		_head->_srl_children[0] = _tail;
		_tail->_srl_children[0] = _head;
	}

	void push_back(T elem) {
		std::cout << "Start pushing " << elem << std::endl;
		_srl_node* _new_node = new _srl_node(elem);
		_tail->color = 0; // mark the tail
		_srl_node _end_node = _tail->_srl_children[0];
		int i = 0;
		if (_end_node._srl_children[0]->color != 0) i = 1;
		// _end_node._srl_children[i] = _tail
		_end_node->_srl_children[i] = _new_node;
		_new_node->_srl_children[0] = _tail; //
		_tail->_srl_children[0] = _new_node;
		_new_node._srl_children[1] = _end_node;
		_size++;
		_tail->color = -1; // unmark the tail
		std::cout << "Successfully pushing " << elem << std::endl;

	}

// precondition the list must be non-empty, i.e, _size != 0
	void remove_back() {
		_srl_node _end_node = _tail._srl_children[0]; // the node to be removed
		_end_node.color = 0;
		// recall _end_node._srl_children[0] = _tail
		_srl_node _prev_end_node = _end_node._srl_children[1];
		int i = 0;
		if (_prev_end_node._srl_children[0]->color != 0) i = 1;
		// _prev_end_node._srl_children[i] = _end_node; 
		if (i != 0) {
			_prev_end_node._srl_children[1] = _prev_end_node._srl_children[0];
 		}
		_prev_end_node._srl_children[0] = _tail;
		_tail._srl_children[0] = _prev_end_node;
		delete _end_node;
	}


	void push_front(T elem) {
		_srl_node _new_node = new _srl_node(elem);
//		_head.color = 0; // mark the head
		_srl_node _start_node = _head._srl_children[0];

		_head._srl_children[0] = _new_node;
		_new_node._srl_children[0] = head;

		_start_node._srl_children[0] = _new_node;
		_new_node._srl_children[1] = _start_node;
		_size++;

//		_head.color = -1; // unmark the head

	}

// precondition: the list must be non-empty
	void remove_font() {
		_srl_node _start_node = _head._srl_children[0]; // the node to be removed
		_start_node.

	}

	bool is_empty() {
		return _size == 0 ? true : false;
	}
	int size() {
		return _size;
	}

	void print() {
		_srl_node *_it = _head->_srl_children[0];;
		_it->color = 0;
		int i = 0;
		while (_it->_srl_children[1] != _tail) {
			std::cout << _it->_data << "\t";
			if (_it->_srl_children[0]->color != -1) i = 1;
			//_it->->_srl_children[i] is the unvisited child
			_it = _it->_srl_children[i];
			_it->color = 0;
		}
		std::cout << std::endl;
		// reset the color
		_it = _head->_srl_children[0];;
		_it->color = -1;
		int i = 0;
		while (_it->_srl_children[1] != _tail) {
			//std::cout << _it->_data << "\t";
			if (_it->_srl_children[0]->color != 0) i = 1;
			//_it->->_srl_children[i] must have the color updated
			_it = _it->_srl_children[i];
			_it->color = -1;
		}
	}
};