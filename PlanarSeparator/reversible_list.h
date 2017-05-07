#pragma once
#include<iostream>

template <class T>
struct _srl_node {

	T _data{};
	std::array<_srl_node<T>*, 2> _neighbors;
	int _color;

	_srl_node();
	_srl_node(T arg_data);
};

template <class T>
_srl_node<T>::_srl_node() {
	_color = -1;
	_neighbors[0] = nullptr;
	_neighbors[1] = nullptr;
}
template <class T>
_srl_node<T>::_srl_node(T arg_data = 0) : _data(arg_data) {
	_color = -1;
	_neighbors[0] = nullptr;
	_neighbors[1] = nullptr;
}


//	We always gurantee that head->neighbros[1] = null and tail->neighbors[1] = null
template<typename T>
class srlist {
	_srl_node<T>* _head;
	_srl_node<T>* _tail;
	int _size;
public:
	srlist();
	void push_back(T elem);
	void remove_back();
	void push_front(T elem);
	void remove_font();
	void reverse();
	bool is_empty();
	int size();
	void print();

	// TODO: implement splice();

};

template<typename T>
srlist<T>::srlist() {	// default constructor
	_head = new _srl_node<T>();
	_tail = new _srl_node<T>();
	_head->_neighbors[0] = _tail;
	_tail->_neighbors[0] = _head;
	_size = 0;
}


template<typename T>
void srlist<T>::push_back(T elem) {
	//	std::cout << "Start pushing " << elem << std::endl;
	_srl_node<T>* _new_node = new _srl_node<T>(elem);
	_srl_node<T>* _end_node = _tail->_neighbors[0];
	int i = 0;
	if (_end_node->_neighbors[0] != _tail) i = 1;
	// _end_node._neighbors[i] = _tail
	_end_node->_neighbors[i] = _new_node;
	_new_node->_neighbors[1] = _end_node;
	_new_node->_neighbors[0] = _tail; //
	_tail->_neighbors[0] = _new_node;
	_size++;
//	std::cout << "Successfully pushing " << elem << std::endl;
}

// precondition the list must be non-empty, i.e, _size != 0
template<typename T>
void srlist<T>::remove_back() {
	_srl_node<T> *_end_node = _tail->_neighbors[0]; // the node to be removed
	_end_node->_color = 0;
	int i = 0;
	if (_end_node->_neighbors[0] != _tail) i = 1;
	_srl_node<T>* _prev_end_node = _end_node->_neighbors[1 - i];
	i = 0;
	if (_prev_end_node->_neighbors[0]->_color != 0) i = 1;
	// _prev_end_node->neighbors[i] = _end_node; 
	_prev_end_node->_neighbors[i] = _tail;
	_tail->_neighbors[0] = _prev_end_node;
	_size--;
	//std::cout << "Removing " << _end_node->_data << std::endl;
	delete _end_node;
}


template<typename T>
void srlist<T>::push_front(T elem){
	_srl_node<T> *_new_node = new _srl_node<T>(elem);
	_srl_node<T>* _start_node = _head->_neighbors[0];
	std::cout << "Start node: " << _start_node->_data << std::endl;
	_head->_neighbors[0] = _new_node;
	_new_node->_neighbors[0] = _head;
	_new_node->_neighbors[1] = _start_node;
	int i = 0;
	if (_start_node->_neighbors[0] != _head) i = 1;
	// _start_node->neighbors[i] = head
	_start_node->_neighbors[i] = _new_node;
	_size++;
//	std::cout << "Successfully pushing " << _head->_neighbors[0]->_data << " to the front" << std::endl;
}

// precondition: the list must be non-empty
template<typename T>
void srlist<T>::remove_font() {
	_srl_node<T>* _start_node = _head->_neighbors[0];
	_start_node->_color = 0;
	int i = 0;
	if (_start_node->_neighbors[0] != _head) i = 1;
	// _start_node->neighbors[i] = _head
	_srl_node<T> * _next_of_start_node = _start_node->_neighbors[1 - i];
	_head->_neighbors[0] = _next_of_start_node;
	i = 0;
	if (_next_of_start_node->_neighbors[0]->_color != 0) i = 1;
	//_next_of_start_node->_neighbors[i] = _start_node
	_next_of_start_node->_neighbors[i] = _head;
	_size--;
//	std::cout << "Removing " << _start_node->_data << " from the front" << std::endl;
	delete _start_node;
}

template<typename T>
void srlist<T>::reverse() {
	_srl_node<T>* tmp = _head;
	_head = _tail;
	_tail = tmp;
}
template<typename T>
bool srlist<T>::is_empty() {
	return _size == 0 ? true : false;
}

template<typename T>
int srlist<T>::size() {
	return _size;
}

template <typename T>
void srlist<T>::print() {
	_srl_node<T> *_it = _head->_neighbors[0];
	_head->_color = 0;
	_it->_color = 0;
	int i = 0;
	while (_it->_neighbors[1] != nullptr) {
		std::cout << _it->_data << "\t";
		i = 0;
		if (_it->_neighbors[0]->_color == 0) i = 1;
		//_it->_neighbors[i] is the unvisited child
		_it = _it->_neighbors[i];
		_it->_color = 0;
	}
	std::cout << std::endl;
	// reset the color
	_head->_color = -1;
	_it = _head->_neighbors[0];
	_it->_color = -1;
	while (_it->_neighbors[1] != nullptr) {
		i = 0;
		if (_it->_neighbors[0]->_color == -1) i = 1;
		//_it->->_neighbors[i] must have the color updated
		_it = _it->_neighbors[i];
		_it->_color = -1;
	}
}