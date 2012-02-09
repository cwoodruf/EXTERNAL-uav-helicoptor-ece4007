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
#include <iostream>
#include "../datastructures/circularBuffer.h"


using namespace std;


int main() {

	cout << "Testing Constructors...";
	CircularBuffer<int> cb;
	CircularBuffer<int> cb1(64);

	try {
		CircularBuffer<int> cb2(123);
		cout << "FAIL" << endl;
	} catch(...) {
		cb1.enqueue(123);
		CircularBuffer<int> cb3(cb1);

		int c1,c3;
		cb1.dequeue(c1); cb3.dequeue(c3);
		if(c1 == c3) {
			cout << "PASS" << endl;
		} else {
			cout << "FAIL" << endl;
		}
	}


	cout << "Testing isEmpty...";
	if(cb.isEmpty()) {
		cout << "PASS" << endl;
	} else {
		cout << "FAIL" << endl;
	}

	cout << "Testing length...";
	if(cb.length() == 31) {
		cout << "PASS" << endl;
	} else {
		cout << "FAIL" << endl;
	}

	cout << "Testing queue...";
	for(int i=0;i<cb.length();++i) {
		cb.enqueue(i);
	}

	if(cb.numQueued() != cb.length()) {
		cout << "FAIL" << endl;
	} else {
		cout << "PASS" << endl;
	}

	cout << "Testing isFull...";
	if(cb.isFull()) {
		cout << "PASS" << endl;
	} else {
		cout << "FAIL" << endl;
	}

	cout << "Testing numQueued...";
	if(cb.numQueued() == 31) {
		cout << "PASS" << endl;
	} else {
		cout << "FAIL" << endl;
	}

	cout << "Testing peek...";
	int v;
	cb.peek(10,v);
	if(v == 10) {
		cout << "PASS" << endl;
	} else {
		cout << "FAIL" << endl;
	}

	cout << "Testing dequeue...";
	int a = 0,i = 0;
	bool pass = true;
	while(cb.numQueued()) {
		cb.dequeue(a);
		if(a != i++) {
			pass = false;
			break;
		}
	}

	if(pass) {
		cout << "PASS" << endl;
	} else {
		cout << "FAIL" << endl;
	}


	cout << "Testing clear...";
	cb.clear();
	if(cb.numQueued() == 0) {
		cout << "PASS" << endl;
	} else {
		cout << "FAIL" << endl;
	}


	cout << "Testing resize...";
	cb.resize(64);
	if(cb.length() == 63) {
		if(cb.resize(123) == -1) {
			cout << "PASS" << endl;
		} else {
			cout << "FAIL" << endl;
		}
	} else {
		cout << "FAIL" << endl;
	}
	return 0;
}
