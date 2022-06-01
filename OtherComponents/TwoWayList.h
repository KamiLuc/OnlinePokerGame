#pragma once

#ifndef _TWO_WAY_LIST
#define _TWO_WAY_LIST
#include <fstream>
#include <memory>
#include <iostream>
#include <exception>


//EXCEPTION
///////////////////////////////////////////////////////////////////////////////////////////////
																							 //
class Two_way_list_exception : public std::exception {                                       //
public:																						 //
	Two_way_list_exception(const char* n = "unknown exception") : std::exception(n) {};		 //
};																							 //
///////////////////////////////////////////////////////////////////////////////////////////////

template<typename type>
class Two_way_list
{
private:
	void hidden_push_back(); //used in resize

	struct Node {
	public:
		type value;
		std::shared_ptr<struct Node> next;
		std::weak_ptr<struct Node> previous;
		friend bool operator==(const Node& a, const Node& b) {
			return &a.value == &b.value;
		}
		friend bool operator!=(const Node& a, const Node& b) {
			return &a.value != &b.value;
		}
		friend bool operator<(const Node& a, const Node& b) {
			return a.value < b.value;
		}
	};

	void erase(std::shared_ptr<Node>& ptr); //used in erase(type value)

	std::shared_ptr<struct Node> right;
	std::shared_ptr<struct Node> left;
	int how_many = 0;
	//ITERATOR 
	//////////////////////////////////////////////////////////////////////////////////////////
public:
	class iterator {
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = type;
		using pointer = std::weak_ptr<struct Node>;
		using reference = type&;
		pointer m_ptr;
		iterator(pointer ptr) : m_ptr(ptr) {}
		reference operator* () const {
			std::shared_ptr<Node> temp(m_ptr);
			return temp->value;
		}
		pointer operator->() {
			return m_ptr;
		}
		iterator& operator++() {
			std::shared_ptr<Node> temp(m_ptr);
			m_ptr = temp->next;
			return *this;
		}
		iterator operator++(int) {
			iterator temp = *this;
			++(*this);
			return temp;
		}
		iterator& operator--() {
			std::shared_ptr<Node> temp(m_ptr);
			m_ptr = temp->previous;
			return *this;
		}
		iterator operator--(int) {
			iterator temp = *this;
			--(*this);
			return temp;
		}

		friend bool operator== (const iterator& a, const iterator& b) {
			auto temp1 = a.m_ptr.lock();
			auto temp2 = b.m_ptr.lock();
			return (&temp1->value == &temp2->value);
		}
		friend bool operator!= (const iterator& a, const iterator& b) {
			auto temp1 = a.m_ptr.lock();
			auto temp2 = b.m_ptr.lock();
			return (&temp1->value != &temp2->value);
		}


	};
	//////////////////////////////////////////////////////////////////////////////////////////

public:
	Two_way_list();
	Two_way_list(int s, type v);
	Two_way_list(Two_way_list& t);
	Two_way_list(Two_way_list&& t);
	Two_way_list(std::initializer_list<type> ilist);
	int size(void) const { return how_many; }
	void push_back(const type& value);
	void pop_back(void);
	void push_front(const type& value);
	void pop_front(void);
	type& front(void);
	const type& front(void) const;
	type& back(void);
	const type& back(void) const;
	bool is_empty(void) const;
	void show_all(void) const;
	void assign(int count, const type& v);
	void assign(std::initializer_list<type> ilist);
	void clear(void) noexcept;
	void resize(int count);
	void sort(bool(*compare_function)(type&, type&));
	void sort(void);
	void write_into_file(const char* filename) const;
	void read_from_file(const char* filename);
	void erase(int where);
	void erase_value(const type& value);
	void push_anywhere(const type& value, int where);
	iterator begin() const { return iterator(left); }
	iterator end() const { return iterator(right->next); }
	bool find(const type& value) const;

	~Two_way_list();
	//OPERATORS
	//////////////////////////////////////////////////////////////////////////////////////////
	const Two_way_list& operator=(const Two_way_list& t) {
		if (this == &t) {
			return *this;
		}
		right = nullptr;
		left = nullptr;
		std::shared_ptr<Node> temp = t.left;
		while (temp != nullptr) {
			push_back(temp->value);
			temp = temp->next;
		}
		return *this;
	}
	Two_way_list& operator=(const Two_way_list&& t) {
		if (this == &t) {
			return *this;
		}
		right = t.right;
		left = t.left;
		how_many = t.how_many;
		return *this;
	}
	Two_way_list& operator=(std::initializer_list<type> ilist) {
		for (auto element : ilist) {
			push_back(element);
		}
	}

	type& operator[](int position) {
		if (position < 0 or position >= how_many) {
			throw Two_way_list_exception("Out of range argument");
		}
		std::shared_ptr<Node> temp = left;
		for (int i = 0; i < position; i++) {
			temp = temp->next;
		}
		return temp->value;
		temp = nullptr;
	}

	type& operator[](int position) const {
		if (position < 0 or position >= how_many) {
			throw Two_way_list_exception("Out of range argument");
		}
		std::shared_ptr<Node> temp = left;
		for (int i = 0; i < position; i++) {
			temp = temp->next;
		}
		return temp->value;
	}

	//FRIEND
	//////////////////////////////////////////////////////////////////////////////////////////
	friend std::ostream& operator<<(std::ostream& s, const Two_way_list& t) {
		t.show_all();
		return s;
	}

	friend void swap(Two_way_list& l1, Two_way_list& l2) {
		Two_way_list<type> l3 = std::move(l1);
		l1 = std::move(l2);
		l2 = std::move(l3);
	}
};

template<typename type>
Two_way_list<type>::Two_way_list()
{
	right = nullptr;
	left = nullptr;
}

template<typename type>
Two_way_list<type>::Two_way_list(Two_way_list& t)
{
	right = nullptr;
	left = nullptr;
	std::shared_ptr<Node> temp = t.left;
	while (temp != nullptr) {
		push_back(temp->value);
		temp = temp->next;
	}
}

template<typename type>
Two_way_list<type>::Two_way_list(int s, type v)
{
	for (int i = 0; i < s; i++) {
		push_back(v);
	}
}

template<typename type>
Two_way_list<type>::Two_way_list(Two_way_list&& t)
{
	how_many = t.how_many;
	t.how_many = 0;
	left = t.left;
	right = t.right;
	t.left = t.right = nullptr;
}

template<typename type>
Two_way_list<type>::Two_way_list(std::initializer_list<type> ilist)
{
	for (auto element : ilist) {
		push_back(element);
	}
}

template<typename type>
void Two_way_list<type>::push_back(const type& v)
{
	std::shared_ptr<Node> new_node(new Node);
	if (is_empty()) {
		std::shared_ptr<Node> new_node(new Node);
		new_node->value = v;
		right = new_node;
		left = new_node;
		how_many++;
	}
	else {
		std::shared_ptr<Node> new_node(new Node);
		new_node->value = v;
		right->next = new_node;
		new_node->previous = right;
		right = new_node;
		new_node = nullptr;
		how_many++;
	}
}

template<typename type>
void Two_way_list<type>::pop_back(void)
{
	if (how_many == 0) {
		throw Two_way_list_exception("Pop back used in empty list!");
	}
	right = std::shared_ptr<Node>(right->previous);
	right->next = nullptr;
	how_many--;
}

template<typename type>
void Two_way_list<type>::push_front(const type& v)
{
	if (is_empty()) {
		push_back(v);
	}
	else {
		std::shared_ptr<Node> new_node(new Node);
		new_node->value = v;
		new_node->next = left;
		left->previous = new_node;
		left = new_node;
		new_node = nullptr;
		how_many++;
	}
}

template<typename type>
void Two_way_list<type>::pop_front(void)
{
	if (how_many == 0) {
		throw Two_way_list_exception("Pop front used in empty list!");
	}
	left = left->next;
	how_many--;
}

template<typename type>
bool Two_way_list<type>::is_empty() const
{
	if (how_many == 0) {
		return true;
	}
	return false;
}

template<typename type>
Two_way_list<type>::~Two_way_list()
{
	left = right = nullptr;
}

template<typename type>
void Two_way_list<type>::show_all(void) const
{
	std::shared_ptr<Node> t = left;
	for (int i = 0; i < how_many; i++) {
		std::cout << t->value << "  ";
		t = t->next;
	}
	std::cout << std::endl;
}

template<typename type>
void Two_way_list<type>::assign(int count, const type& v)
{
	if (count < 0) {
		throw Two_way_list_exception("Bad assign, count argument < 0");
	}
	std::shared_ptr<Node> temp = left;
	if (count < how_many) {
		temp->value = v;
		for (int i = 0; i < count - 1; i++) {
			temp = temp->next;
			temp->value = v;
		}
		right = temp;
		right->next = nullptr;
	}
	else {
		int x = count - how_many;
		for (int i = 0; i < how_many; i++) {
			temp->value = v;
			temp = temp->next;
		}
		for (int i = 0; i < x; i++) {
			push_back(v);
		}
	}
	temp = nullptr;
	how_many = count;
}

template<typename type>
void Two_way_list<type>::assign(std::initializer_list<type> ilist)
{
	if (ilist.size() <= 0) {
		throw Two_way_list_exception("Bad assign initializer_list.size() < 0");
	}
	auto pointer = ilist.begin();
	std::shared_ptr<Node> temp = left;
	if (ilist.size() < how_many) {
		temp->value = *pointer;
		pointer++;
		for (int i = 0; i < ilist.size() - 1; i++) {
			temp = temp->next;
			temp->value = *pointer;
			pointer++;
		}
		right = temp;
		right->next = nullptr;
	}
	else {
		int x = ilist.size() - how_many;
		for (int i = 0; i < how_many; i++) {
			temp->value = *pointer;
			pointer++;
			temp = temp->next;
		}
		for (int i = 0; i < x; i++) {
			push_back(*pointer);
			pointer++;
		}
	}
	temp = nullptr;
	how_many = ilist.size();

}

template<typename type>
void Two_way_list<type>::clear(void) noexcept
{
	left = right = nullptr;
	how_many = 0;
}

template<typename type>
void Two_way_list<type>::resize(int count)
{
	if (count < 0) {
		throw Two_way_list_exception("Bad resize, count argument < 0!");
	}
	if (count > how_many) {
		int x = count - how_many;
		for (int i = 0; i < x; i++) {
			hidden_push_back();
		}
	}
	std::shared_ptr<Node> temp = left;
	for (int i = 0; i < count - 1; i++) {
		temp = temp->next;
	}
	how_many = count;
	right = temp;
	right->next = nullptr;
	temp = nullptr;
}

template<typename type>
void Two_way_list<type>::hidden_push_back()
{
	std::shared_ptr<Node> new_node(new Node);
	if (is_empty()) {
		std::shared_ptr<Node> new_node(new Node);
		right = new_node;
		left = new_node;
		how_many++;
	}
	else {
		std::shared_ptr<Node> new_node(new Node);
		right->next = new_node;
		new_node->previous = right;
		right = new_node;
		new_node = nullptr;
		how_many++;
	}
}



template<typename type>
type& Two_way_list<type>::front()
{
	if (how_many == 0) {
		throw Two_way_list_exception("Bad front, empty list!");
	}
	return (left->value);
}

template<typename type>
const type& Two_way_list<type>::front(void) const
{
	if (how_many == 0) {
		throw Two_way_list_exception("Bad front, empty list!");
	}
	return (right->value);
}

template<typename type>
type& Two_way_list<type>::back(void)
{
	if (how_many == 0) {
		throw Two_way_list_exception("Bad back, empty list!");
	}
	return (right->value);
}

template<typename type>
const type& Two_way_list<type>::back(void) const
{
	if (how_many == 0) {
		throw Two_way_list_exception("Bad back, empty list!");
	}
	return (right->value);
}

template<typename type>
void Two_way_list<type>::sort(bool (*compare_function)(type&, type&))
{
	if (!is_empty()) {
		std::shared_ptr<Node> temp_left = left;
		std::shared_ptr<Node> temp_pointer;
		type temp_value;
		while (temp_left->next != nullptr) {
			temp_pointer = temp_left->next;
			while (temp_pointer != nullptr) {
				if (compare_function(temp_pointer->value, temp_left->value)) {
					temp_value = temp_pointer->value;
					temp_pointer->value = temp_left->value;
					temp_left->value = temp_value;
				}
				temp_pointer = temp_pointer->next;
			}
			temp_left = temp_left->next;
		}
	}
}

template<typename type>
void Two_way_list<type>::sort()
{
	sort([](type& a, type& b) {return a > b; });
}

template<typename type>
void Two_way_list<type>::write_into_file(const char* filename) const
{
	std::ofstream myfile;
	myfile.open(filename);
	if (myfile.is_open()) {
		std::shared_ptr<Node> temp = left;
		while (temp != nullptr) {
			myfile << temp->value << std::endl;
			temp = temp->next;
		}
		myfile.close();
	}
	else
		throw Two_way_list_exception("Can't write into file!");
}

template<typename type>
void Two_way_list<type>::read_from_file(const char* filename)
{
	std::ifstream myfile;
	myfile.open(filename);
	if (myfile.is_open()) {
		type t;
		while (myfile >> t) {
			push_back(t);
		}
		myfile.close();
	}
	else
		throw Two_way_list_exception("Can't open file!");
}

template<typename type>
void Two_way_list<type>::erase(int where)
{
	if (where < 0 or where >= how_many) {
		throw Two_way_list_exception("Out of range argument!");
	}
	else if (where == 0) {
		pop_front();
	}
	else if (where == (how_many - 1)) {
		pop_back();
	}
	else {
		std::shared_ptr<Node> temp = left;
		for (int i = 0; i < where; i++) {
			temp = temp->next;
		}
		std::shared_ptr<Node> temp2 = temp->previous.lock();
		temp2->next = temp->next;
		temp->next->previous = temp2;
		how_many--;
		temp2 = temp = nullptr;
	}
}

template<typename type>
void Two_way_list<type>::push_anywhere(const type& value, int where)
{
	if (where < 0 or where > how_many) {
		throw Two_way_list_exception("Out of range argument!");
	}
	if (where == 0) {
		push_front(value);
	}
	else if (where == how_many) {
		push_back(value);
	}
	else {
		std::shared_ptr<Node> temp = left;
		std::shared_ptr<Node> new_node(new Node);
		new_node->value = value;
		for (int i = 0; i < (where - 1); i++) {
			temp = temp->next;
		}
		if (temp->next != nullptr) {
			new_node->next = temp->next;
			temp->next->previous = new_node;

		}
		temp->next = new_node;
		new_node->previous = temp;
		how_many++;
		temp = new_node = nullptr;
	}
}


template<typename type>
void Two_way_list<type>::erase_value(const type& value)
{
	std::shared_ptr<Node> temp = left;
	while (temp != nullptr) {
		if (temp->value == value) {
			erase(temp);
		}
		else {
			temp = temp->next;
		}
	}
}

template<typename type>
void Two_way_list<type>::erase(std::shared_ptr<Node>& ptr)
{
	if (ptr == left) {
		ptr = ptr->next;
		pop_front();
	}
	else if (ptr == right) {
		ptr = ptr->next;
		pop_back();
	}
	else {
		std::shared_ptr<Node> temp = ptr->previous.lock();
		temp->next = ptr->next;
		ptr->next->previous = temp;
		ptr = temp->next;
		how_many--;
	}
}

template<typename type>
inline bool Two_way_list<type>::find(const type& value) const
{
	std::shared_ptr<Node> temp = left;
	while (temp != nullptr) {
		if (temp->value == value) {
			return true;
		}
		temp = temp->next;
	}
	return false;
}

#endif