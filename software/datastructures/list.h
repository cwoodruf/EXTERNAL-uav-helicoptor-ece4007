/*
* Copyright (c) 2012 Joey Yore
*
* Permission is hereby granted, free of charge, to any person obtaining a 
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation 
* the rights to use, copy, modify, merge, publish, distribute, sublicense, 
* and/or sell copies of the Software, and to permit persons to whom the 
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included 
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
* OTHER DEALINGS IN THE SOFTWARE.
*/


#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <iostream>
using namespace std;

template <typename T> class List;
template <typename T> class ListIterator;

template <typename T>
struct ListNode {
	struct ListNode *prev;
	struct ListNode *next;
	T data;
};

template <typename T>
class ListIterator {

	private:
		List<T> *list;
		ListNode<T> *current;
		friend class List<T>;

	public:

		ListIterator(List<T> *l) {
			list = l;
			current = l->head;
		}

		void front() {
			current = list->head;
		}		

		void back() {
			current = list->tail;
		}

		T& operator*() const {
			return current->data;
		}

		ListIterator operator++(int) {
			ListIterator tmp(*this);
			if(current) {
				current = current->next;
			}
			return tmp;
		}

		ListIterator operator++() {
			if(current) {
				current = current->next;
			}
			return *this;
		}

		ListIterator operator--(int) {
			ListIterator tmp(*this);
			if(current) {
				current = current->prev;
			}
			return tmp;
		}

		ListIterator operator--() {
			if(current) {
				current = current->prev;
			}
			return *this;
		}

		bool operator==(const ListIterator &other) {
			return (current == other.current);
		}

		bool operator!=(const ListIterator &other) {
			return (current != other.current);
		}
};

template <typename T>
class List {

	private:
		ListNode<T> *head;
		ListNode<T> *tail;
		int size;

		friend class ListIterator<T>;

	public:

		List() : head(NULL), tail(NULL), size(0) {};
		List(const List<T> &cpy);
		~List() {clear();}

		bool isEmpty() {return (size==0);}
		int push(const T data);
		int pop(T &data);
		int push_front(const T data);
		int pop_front(T &data);
		int insert(const T data,int index);
		int remove(T &data,int index);
		int get(T &data,int index);
		void clear();
		void reverse();
		void cat(const List<T> &list);

		ListIterator<T> getIterator() {
			ListIterator<T> tmp(this);
			return tmp;
		}

		ListIterator<T> begin() {
			ListIterator<T> tmp(this);
			tmp.front();
			return tmp;
		}
		ListIterator<T> end() {
			ListIterator<T> tmp(this);
			tmp.back();
			return tmp;	
		}
};

template <typename T>
List<T>::List(const List<T> &cpy) {

	if(size) clear();

	ListNode<T> node = head;
	while(node != NULL) {
		push(node->data);
		node = node->next;
	}
}

template <typename T>
int List<T>::push(const T data) {
	ListNode<T> *newNode = new ListNode<T>;

	if(tail != NULL) {
		tail->next = newNode;
	} else {
		head = newNode;
	}

	newNode->prev = tail;
	newNode->data = data;
	newNode->next = NULL;
	tail = newNode;
	++size;

	return 0;
}

template <typename T>
int List<T>::pop(T &data) {
	if(tail != NULL) {
		data = tail->data;
		ListNode<T> *oldTail = tail;

		if((tail = tail->prev) != NULL) {
			tail->next = NULL;
		} else {
			head = NULL;
		}

		delete oldTail; oldTail = NULL;
		--size;

		return 0;
	}

	return -1;
}

template <typename T>
int List<T>::push_front(const T data) {
	ListNode<T> *newNode = new ListNode<T>;

	if(head != NULL) {
		head->prev = newNode;
	} else {
		tail = newNode;
	}

	newNode->next = head;
	newNode->data = data;
	newNode->prev = NULL;
	head = newNode;

	++size;

	return 0;
};

template <typename T>
int List<T>::pop_front(T &data) {

	if(head != NULL) {
		data = head->data;
		ListNode<T> *oldHead = head;

		if((head = head->next) != NULL) {
			head->prev = NULL;
		} else {
			tail = NULL;
		}

		delete oldHead; oldHead = NULL;
		--size;

		return 0;
	}

	return -1;
}


template <typename T>
int List<T>::insert(const T data,int index) {

	if(index < 0 || index >= size) return -1;

	ListNode<T> *node = head;
	ListNode<T> *newNode = new ListNode<T>;
	newNode->data = data;

	while(index--) {
		node = node->next;
	}

	if(node->prev != NULL) {
		node->prev->next = newNode;
	} else {
		head = newNode;
	}

	newNode->prev = node->prev;
	newNode->next = node;
	node->prev = newNode;
	
	++size;	

	return 0;
}

template <typename T>
int List<T>::remove(T &data,int index) {

	if(index < 0 || index >= size) return -1;

	ListNode<T> *node = head;
	while(index--) {
		node = node->next;
	}

	data = node->data;

	if(node->prev != NULL) {
		node->prev->next = node->next;
	} else {
		head = node->next;
	}

	delete node; node = NULL;
	--size;

	return 0;
}

template <typename T>
int List<T>::get(T &data,int index) {
	if(index < 0 || index >= size) return -1;

	ListNode<T> *node = head;
	while(index--) {
		node = node->next;
	}

	return node->data;
}

template <typename T>
void List<T>::clear() {
	ListNode<T> *oldHead;

	while(head != NULL) {
		oldHead = head;
		head = head->next;
		delete oldHead; oldHead = NULL;
	}

	head = tail = NULL;
	size = 0;
}

template <typename T>
void List<T>::reverse() {

	if(size > 1) {
		ListNode<T> *node = head;
		ListNode<T> *tmp = tail;
		tail = node;
		head = tmp;

		while(node != NULL) {
			tmp = node->next;
			node->next = node->prev;
			node->prev = tmp;
			node = tmp;
		}
	}
}

template <typename T>
void List<T>::cat(const List<T> &list) {
	ListNode<T> *node = list.head;

	while(node != NULL) {
		push(node->data);
		node = node->next;
	}
}

#endif
