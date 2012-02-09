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
#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H


template <typename T>
class CircularBuffer {

	private:
		T *buffer;
		int size;
		int head;
		int tail;

	public:

		//Constructors
		CircularBuffer() {
			size = 32;
			buffer = new T[size];
			head = tail = 0;
		}

		CircularBuffer(const int s) {
			if(s < 0 || (s & (s - 1))) {
				throw "CircularBuffer: invalid initialization parameter";
			}

			size = s;
			buffer = new T[size];
			head = tail = 0;
		}

		CircularBuffer(const CircularBuffer& cpy) {

			size = cpy.size;
			buffer = new T[size];

			for(int i=0;i<size;++i) {
				buffer[i] = cpy.buffer[i];				
			}

			head = cpy.head;
			tail = cpy.tail;
		}

		//Destructor
		~CircularBuffer() {
			delete[] buffer;
			size = 1;
			head = tail = 0;
		}

		//Member Functions
		int enqueue(const T &p);
		int dequeue(T &p);
		int peek(const int i, T &p);
		int resize(const int s);

		//Inline Functions
		inline void clear() {
			head = tail = 0;
		}

		inline int length() {
			return (size - 1);
		}

		inline int numQueued() {
			return ((head > tail) ? size : 0) + tail - head;
		}

		inline bool isEmpty() {
			return (head == tail);
		}

		inline bool isFull() {
			return (((tail + 1) & (size - 1)) == head);
		}
};

template <typename T>
int CircularBuffer<T>::enqueue(const T &p) {

	if(isFull()) {
		return -1;
	}

	buffer[tail] = p;
	tail = (tail + 1) & (size - 1);
	return 0;
}

template <typename T>
int CircularBuffer<T>::dequeue(T &p) {

	if(isEmpty()) {
		return -1;
	}

	p = buffer[head];
	head = (head + 1) & (size - 1);
	return 0;
}

template <typename T>
int CircularBuffer<T>::peek(const int i, T &p) {

	int j = 0;
	int k = head;
	while(k != tail) {
		if(j == i) break;
		++j;

		k = (k + 1) & (size - 1);
	}

	if(k == tail) {
		return -1;
	}
	p = buffer[k];

	return 0;
}

template <typename T>
int CircularBuffer<T>::resize(const int s) {

	if((s <= 0) || (s & (s - 1))) {
		return -1;
	}

	delete[] buffer;
	buffer = new T[s];
	size = s;
	head = tail = 0;

	return 0;
}

#endif
